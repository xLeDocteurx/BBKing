#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_http_server.h>
#include <esp_netif.h>
#include <esp_netif_defaults.h>

#include <WifiConfig.h>
#include <readJsonFile.h>
#include <getInstrumentSampleIndex.h>

#include <GlobalVars.h>

State *statePointer;

/*
 * Structure holding server handle
 * and internal socket fd in order
 * to use out of request send
 */
struct async_resp_arg
{
    httpd_handle_t hd;
    int fd;
};

/*
 * async send function, which we put into the httpd work queue
 */
static void ws_async_send(void *arg)
{
    static const char *data = "Async data";
    // struct async_resp_arg *resp_arg = arg;
    // httpd_handle_t hd = resp_arg->hd;
    // int fd = resp_arg->fd;
    httpd_handle_t hd = ((async_resp_arg *)arg)->hd;
    int fd = ((async_resp_arg *)arg)->fd;
    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.payload = (uint8_t *)data;
    ws_pkt.len = strlen(data);
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    httpd_ws_send_frame_async(hd, fd, &ws_pkt);
    // free(resp_arg);
    free(arg);
}

static esp_err_t trigger_async_send(httpd_handle_t handle, httpd_req_t *req)
{
    // struct async_resp_arg *resp_arg = malloc(sizeof(struct async_resp_arg));
    async_resp_arg *resp_arg = (async_resp_arg *)malloc(sizeof(async_resp_arg));
    if (resp_arg == NULL)
    {
        return ESP_ERR_NO_MEM;
    }
    resp_arg->hd = req->handle;
    resp_arg->fd = httpd_req_to_sockfd(req);
    esp_err_t ret = httpd_queue_work(handle, ws_async_send, resp_arg);
    if (ret != ESP_OK)
    {
        free(resp_arg);
    }
    return ret;
}

static esp_err_t root_handler(httpd_req_t *req)
{
    if (req->method == HTTP_GET)
    {
        // action before ws handshake
        std::string htmlString;
        // bool readJsonFile_ret = readJsonFile("/data/index.html", &htmlString);
        readJsonFile("/data/index.html", &htmlString);
        // httpd_resp_set_status(req, "200");
        httpd_resp_set_type(req, "text/html");
        // httpd_resp_set_hdr();
        httpd_resp_send(req, htmlString.c_str(), HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }
    return ESP_OK;
}
httpd_uri_t root_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = root_handler,
    .user_ctx = NULL,
};

static esp_err_t websocket_handler(httpd_req_t *req)
{
    // beginning of the ws URI handler
    if (req->method == HTTP_GET)
    {
        printf("Handshake done, the new connection was opened\n");
        return ESP_OK;
    }

    httpd_ws_frame_t ws_pkt;
    uint8_t *buf = NULL;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK)
    {
        printf("httpd_ws_recv_frame failed to get frame len with %d\n", ret);
        return ret;
    }

    printf("frame len is %d\n", ws_pkt.len);
    if (ws_pkt.len)
    {
        httpd_ws_frame_t ws_pkt;
        uint8_t *buf = NULL;
        memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
        ws_pkt.type = HTTPD_WS_TYPE_TEXT;
        /* Set max_len = 0 to get the frame len */
        esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
        if (ret != ESP_OK)
        {
            printf("httpd_ws_recv_frame failed with %d\n", ret);
            free(buf);
            return ret;
        }
        printf("Got packet with message: %s\n", ws_pkt.payload);
    }
    printf("Packet type: %d\n", ws_pkt.type);
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strcmp((char *)ws_pkt.payload, "Trigger async") == 0)
    {
        free(buf);
        return trigger_async_send(req->handle, req);
    }

    ret = httpd_ws_send_frame(req, &ws_pkt);
    if (ret != ESP_OK)
    {
        printf("httpd_ws_send_frame failed with %d\n", ret);
    }
    free(buf);
    return ret;
    // return ESP_OK;
}
httpd_uri_t websocket_uri = {
    .uri = "/ws",
    .method = HTTP_GET,
    .handler = websocket_handler,
    .user_ctx = NULL,

    .is_websocket = true,              // Mandatory: set to `true` to handler websocket protocol
    .handle_ws_control_frames = false, // Optional: set to `true` for the handler to receive control packets, too
    .supported_subprotocol = "chat",   // Optional: set supported subprotocol for this handler
};

static esp_err_t state_handler(httpd_req_t *req)
{
    std::string samplesString = "[";
    for (int i = 0; i < statePointer->samples.size(); i++)
    {
        std::string sampleString = "{\"filePath\": \"" + std::string(statePointer->samples[i].filePath) + "\", \"pitch\":" + std::to_string(statePointer->samples[i].pitch) + ", \"volume\":\"" + std::to_string(statePointer->samples[i].volume) + "\"}";
        if (i != 0)
        {
            samplesString += ",";
        }
        samplesString += sampleString;
    }
    samplesString += "]";

    std::string partsString = "[";
    for (int i = 0; i < statePointer->parts.size(); i++)
    {
        std::string partString = "{\"staves\":" + std::to_string(statePointer->parts[i].staves) + ",\"steps\":";
        std::string stepsString = "[";
        for (int j = 0; j < statePointer->parts[i].steps.size(); j++)
        {
            std::string stepString = "[";
            for (int k = 0; k < statePointer->parts[i].steps[j].size(); k++)
            {
                if (k != 0)
                {
                    stepString += ",";
                }
                // stepString += std::to_string(statePointer->parts[i].steps[j][k]);
                statePointer->parts[i].steps[j][k].pitch;
                statePointer->parts[i].steps[j][k].volume;
                stepString += "{\"instrumentIndex\":" + std::to_string(statePointer->parts[i].steps[j][k].instrumentIndex) + ",\"pitch\":" + std::to_string(statePointer->parts[i].steps[j][k].pitch) + ",\"volume\":" + std::to_string(statePointer->parts[i].steps[j][k].volume) + "}";
            }
            stepString += "]";

            if (j != 0)
            {
                stepsString += ",";
            }
            stepsString += stepString;
        }
        stepsString += "]";

        partString += stepsString;

        if (i != 0)
        {
            partsString += ",";
        }
        partsString += partString + "}";
    }
    partsString += "]";

    std::string jsonString =
        "{\"currentSongIndex\":" + std::to_string(statePointer->currentSongIndex) + ",\"songName\":\"" + statePointer->songName + "\",\"songTempo\":" + std::to_string(statePointer->songTempo) + ",\"drumRackSampleFileRefIndex1\":" + std::to_string(statePointer->drumRackSampleFileRefIndex1) + ",\"drumRackSampleFileRefIndex2\":" + std::to_string(statePointer->drumRackSampleFileRefIndex2) + ",\"drumRackSampleFileRefIndex3\":" + std::to_string(statePointer->drumRackSampleFileRefIndex3) + ",\"drumRackSampleFileRefIndex4\":" + std::to_string(statePointer->drumRackSampleFileRefIndex4) + ",\"drumRackSampleFileRefIndex5\":" + std::to_string(statePointer->drumRackSampleFileRefIndex5) + ",\"drumRackSampleFileRefIndex6\":" + std::to_string(statePointer->drumRackSampleFileRefIndex6) + ",\"drumRackSampleFileRefIndex7\":" + std::to_string(statePointer->drumRackSampleFileRefIndex7) + ",\"samples\":" + samplesString + ",\"currentPartIndex\":" + std::to_string(statePointer->currentPartIndex) + ",\"currentPartInstrument\":" + std::to_string(statePointer->currentPartInstrumentIndex) + ",\"parts\":" + partsString + "}";
    httpd_resp_set_type(req, "text/json");
    httpd_resp_send(req, jsonString.c_str(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}
httpd_uri_t state_uri = {
    .uri = "/state",
    .method = HTTP_GET,
    .handler = state_handler,
    .user_ctx = NULL};

static esp_err_t action_handler(httpd_req_t *req)
{
    /* Destination buffer for content of HTTP POST request.
     * httpd_req_recv() accepts char* only, but content could
     * as well be any binary data (needs type casting).
     * In case of string data, null termination will be absent, and
     * content length would give length of string */
    char content[128];

    /* Truncate if content length larger than the buffer */
    size_t recv_size = std::min(req->content_len, sizeof(content));

    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0)
    { /* 0 return value indicates connection closed */
        /* Check if timeout occurred */
        if (ret == HTTPD_SOCK_ERR_TIMEOUT)
        {
            /* In case of timeout one can choose to retry calling
             * httpd_req_recv(), but to keep it simple, here we
             * respond with an HTTP 408 (Request Timeout) error */
            httpd_resp_send_408(req);
        }
        /* In case of error, returning ESP_FAIL will
         * ensure that the underlying socket is closed */
        return ESP_FAIL;
    }
    const std::string contentStr = ((std::string)content);
    printf("content : %s\n", content);
    printf("contentStr.c_str() : %s\n", contentStr.c_str());

    std::size_t pos = ((std::string)content).find("@");
    std::string actionType = ((std::string)content).substr(0, pos);
    std::string actionParameters = ((std::string)content).substr(pos + 1);
    printf("actionType.c_str() : %s\n", actionType.c_str());
    printf("actionParameters.c_str() : %s\n", actionParameters.c_str());

    // TODO : In a separate file to server for other purposes (keyboard, etc...)
    if (actionType == "UPDATESONGNAME")
    {
        statePointer->songName = (char *)actionParameters.c_str();
    }
    else if (actionType == "UPDATETEMPO")
    {
        statePointer->songTempo = stoi(actionParameters);
    }
    else if (actionType == "PLAYFROMSTART")
    {
        statePointer->currentStepIndex = 0;
        statePointer->isPlaying = true;
    }
    else if (actionType == "PLAY")
    {
        statePointer->isPlaying = true;
    }
    else if (actionType == "STOP")
    {
        statePointer->isPlaying = false;
        statePointer->currentStepIndex = 0;
    }
    else if (actionType == "SELECTINSTRUMENT")
    {
        statePointer->currentPartInstrumentIndex = stoi(actionParameters);
    }
    else if (actionType == "UPDATEINSTRUMENTSAMPLEVOLUME")
    {
        int sampleIndex = getInstrumentSampleIndex(statePointer, statePointer->currentPartInstrumentIndex);
        statePointer->samples[sampleIndex].volume = std::stof(actionParameters);
    }
    else if (actionType == "UPDATEINSTRUMENTSAMPLEPITCH")
    {
        int sampleIndex = getInstrumentSampleIndex(statePointer, statePointer->currentPartInstrumentIndex);
        statePointer->samples[sampleIndex].pitch = stoi(actionParameters);
    }
    else if (actionType == "TOGGLEINSTRUMENTSTEP")
    {
        const int stepIndex = stoi(actionParameters);
        statePointer->parts[statePointer->currentPartIndex].steps[stepIndex];

        bool isDrumRackSampleStepActive = false;
        for (int i = 0; i < statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].size(); i++)
        {
            if (statePointer->parts[statePointer->currentPartIndex].steps[stepIndex][i].instrumentIndex == statePointer->currentPartInstrumentIndex)
            {
                int xxxIndex = 0;
                for (int i = 0; i < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); i++)
                {
                    int stepInstrumentIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].instrumentIndex;
                    int sampleIndex = getInstrumentSampleIndex(statePointer, stepInstrumentIndex);
                    if (sampleIndex == statePointer->currentPartInstrumentIndex)
                    {
                        xxxIndex = i;
                    }
                }

                // TODO : Check alternate solution via find
                // Find the position of the number 3
                // std::vector<int>::const_iterator it = std::find(statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].begin(), statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].end(), statePointer->currentPartInstrument);

                // If the number 3 is found, remove it
                std::vector<Step>::iterator it = statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].begin() + xxxIndex;
                if (it != statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].end())
                {
                    isDrumRackSampleStepActive = true;
                    statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].erase(it);
                }
                break;
            }
        }

        if (!isDrumRackSampleStepActive)
        {
            statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].push_back({statePointer->currentPartInstrumentIndex, 0, 1.0});
        }
    }
    else
    {
        printf("Unknown message type\n");
    }

    /* Send a simple response */
    std::string resp = "{\"actionType\":\"" + actionType + "\",\"actionParameters\":\"" + actionParameters + "\"}";
    httpd_resp_send(req, resp.c_str(), HTTPD_RESP_USE_STRLEN);
    // httpd_resp_send(req, content, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}
httpd_uri_t action_uri = {
    .uri = "/action",
    .method = HTTP_POST,
    .handler = action_handler,
    .user_ctx = NULL};

bool initWebServer(State *statePointer_p, httpd_handle_t *server, esp_netif_t *netif)
{
    statePointer = statePointer_p;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    esp_err_t httpd_start_ret = httpd_start(server, &config);
    if (httpd_start_ret != ESP_OK)
    {
        printf("Failed to start HTTP server\n");
        return false;
    }

    esp_err_t httpd_register_uri_handler_ret = httpd_register_uri_handler(*server, &root_uri);
    if (httpd_register_uri_handler_ret != ESP_OK)
    {
        printf("Failed to register root uri handler\n");
        return false;
    }
    httpd_register_uri_handler_ret = httpd_register_uri_handler(*server, &websocket_uri);
    if (httpd_register_uri_handler_ret != ESP_OK)
    {
        printf("Failed to register /ws uri handler\n");
        return false;
    }
    httpd_register_uri_handler_ret = httpd_register_uri_handler(*server, &state_uri);
    if (httpd_register_uri_handler_ret != ESP_OK)
    {
        printf("Failed to register /state uri handler\n");
        return false;
    }
    httpd_register_uri_handler_ret = httpd_register_uri_handler(*server, &action_uri);
    if (httpd_register_uri_handler_ret != ESP_OK)
    {
        printf("Failed to register /action uri handler\n");
        return false;
    }

    return true;
}
