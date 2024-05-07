#include <Init.h>
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

#include <Defs.h>
#include <MyUtils.h>

State *statePointer;

httpd_handle_t *serverPointer;
struct async_resp_arg
{
    httpd_handle_t hd;
    int fd;
};
bool led_state = false;

static void ws_async_send(void *arg)
{
    httpd_ws_frame_t ws_pkt;
    struct async_resp_arg *resp_arg = (async_resp_arg *)arg;
    httpd_handle_t hd = (httpd_handle_t)resp_arg->hd;
    int fd = (int)resp_arg->fd;

    // led_state = !led_state;
    // gpio_set_level(LED_PIN, led_state);

    char buff[4];
    memset(buff, 0, sizeof(buff));
    // std::string jsonString = "";
    // getMachineStateAsJsonString(statePointer, &jsonString);
    sprintf(buff, "%d", led_state);
    // sprintf(buff, jsonString.c_str());

    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.payload = (uint8_t *)buff;
    ws_pkt.len = strlen(buff);
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    static size_t max_clients = CONFIG_LWIP_MAX_LISTENING_TCP;
    size_t fds = max_clients;
    int client_fds[max_clients];

    esp_err_t ret = httpd_get_client_list(*serverPointer, &fds, client_fds);
    if (ret != ESP_OK)
    {
        // printf("? %d\n", ret);
        return;
    }

    for (int i = 0; i < fds; i++)
    {
        int client_info = httpd_ws_get_fd_info(*serverPointer, client_fds[i]);
        if (client_info == HTTPD_WS_CLIENT_WEBSOCKET)
        {
            ret = httpd_ws_send_frame_async(hd, client_fds[i], &ws_pkt);
            if (ret != ESP_OK)
            {
                // printf("?? %d\n", ret);
                return;
            }
        }
    }
    free(resp_arg);
}

static esp_err_t trigger_async_send(httpd_handle_t handle, httpd_req_t *req)
{
    struct async_resp_arg *resp_arg = (async_resp_arg *)malloc(sizeof(struct async_resp_arg));
    resp_arg->hd = req->handle;
    resp_arg->fd = httpd_req_to_sockfd(req);
    return httpd_queue_work(handle, ws_async_send, resp_arg);
}

static esp_err_t root_handler(httpd_req_t *req)
{
    if (req->method == HTTP_GET)
    {
        // action before ws handshake
        std::string htmlString;
        // bool readJsonFile_ret = readJsonFile("/data/index.html", &htmlString);
        getIndexHtml(&htmlString);
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

    if (ws_pkt.len)
    {
        buf = (uint8_t *)calloc(1, ws_pkt.len + 1);
        if (buf == NULL)
        {
            printf("Failed to calloc memory for buf\n");
            return ESP_ERR_NO_MEM;
        }
        ws_pkt.payload = buf;
        ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
        if (ret != ESP_OK)
        {
            printf("httpd_ws_recv_frame failed with %d\n", ret);
            free(buf);
            return ret;
        }
        printf("Got packet with message: %s\n", ws_pkt.payload);
    }

    printf("frame len is %d\n", ws_pkt.len);

    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strcmp((char *)ws_pkt.payload, "toggle") == 0)
    {
        free(buf);
        return trigger_async_send(req->handle, req);
    }
    return ESP_OK;
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
    std::string jsonString = "";
    getMachineStateAsJsonString(statePointer, &jsonString);
    httpd_resp_set_type(req, "text/json");
    httpd_resp_send(req, jsonString.c_str(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}
httpd_uri_t state_uri = {
    .uri = "/state",
    .method = HTTP_GET,
    .handler = state_handler,
    .user_ctx = NULL};

static esp_err_t wavFiles_handler(httpd_req_t *req)
{
    std::string jsonString = "";
    getWavFilesAsJsonString(statePointer, &jsonString);
    httpd_resp_set_type(req, "text/json");
    httpd_resp_send(req, jsonString.c_str(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}
httpd_uri_t wavFiles_uri = {
    .uri = "/wavFiles",
    .method = HTTP_GET,
    .handler = wavFiles_handler,
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
    // if (actionType == "UPDATESONGINDEX")
    // {
    // }
    // else
    if (actionType == "UPDATECURRENTMODE")
    {
        statePointer->currentModeIndex = stoi(actionParameters);
    }
    else if (actionType == "UPDATECURRENTSELECTEDSTEP")
    {
        statePointer->currentSelectedStepIndex = stoi(actionParameters);
    }
    else if (actionType == "UPDATESONGNAME")
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

    else if (actionType == "SOLOPART")
    {
        int desiredIndex = stoi(actionParameters);
        for (int i = 0; i < statePointer->instruments.size(); i++)
        {
            // if(statePointer->instruments[i] == i) {
            //     // TODO
            //     // statePointer->samples[statePointer->instruments[i].sampleFileRefIndex].
            // } else {

            // }
        }
    }
    else if (actionType == "MUTEPART")
    {
        int desiredIndex = stoi(actionParameters);
    }
    else if (actionType == "SELECTPART")
    {
        int desiredIndex = stoi(actionParameters);
        if (desiredIndex >= 0 && desiredIndex < statePointer->parts.size())
        {
            statePointer->currentPartIndex = desiredIndex;
            statePointer->currentStaveIndex = 0;
        }
    }
    else if (actionType == "CREATEPART")
    {
        // int desiredIndex = stoi(actionParameters);
        // if (desiredIndex >= 0 && desiredIndex >= statePointer->parts.size())
        // {
        //     statePointer->currentPartIndex = desiredIndex;
        std::vector<std::vector<Step>> newPartSteps = {};
        const int newPartStaves = 1;
        for (int i = 0; i < STATE_PART_STEPS_LENGTH * newPartStaves; i++)
        {
            switch (i)
            {
            default:
                newPartSteps.push_back({});
                break;
            }
        }
        Part newPart = {newPartStaves, newPartSteps};
        statePointer->parts.push_back(newPart);
        statePointer->currentPartIndex += 1;
        statePointer->currentStaveIndex = 0;
        // }
    }
    else if (actionType == "UPDATESTAVENUMBER")
    {
        int desiredStaveNumber = stoi(actionParameters);
        // if (desiredStaveNumber > 0)
        // {
        if (statePointer->currentStaveIndex >= desiredStaveNumber)
        {
            statePointer->currentStaveIndex = desiredStaveNumber - 1;
        }
        statePointer->parts[statePointer->currentPartIndex].staves = desiredStaveNumber;

        statePointer->parts[statePointer->currentPartIndex].steps.resize(STATE_PART_STEPS_LENGTH * statePointer->parts[statePointer->currentPartIndex].staves, {});
        // }
    }
    else if (actionType == "SELECTSTAVE")
    {
        int desiredStaveIndex = stoi(actionParameters);
        if (desiredStaveIndex >= 0 && desiredStaveIndex < statePointer->parts[statePointer->currentPartIndex].staves)
        {
            statePointer->currentStaveIndex = desiredStaveIndex;
        }
    }
    else if (actionType == "SELECTOCTAVE")
    {
        int desiredOctaveIndex = stoi(actionParameters);
        // if (desiredOctaveIndex >= 0 && desiredOctaveIndex <= 8)
        // {
        statePointer->currentOctaveIndex = desiredOctaveIndex;
        // }
    }

    else if (actionType == "SELECTINSTRUMENT")
    {
        int desiredIntrumentIndex = stoi(actionParameters);
        // if (desiredIntrumentIndex >=0 && < 7)
        // {
        statePointer->currentPartInstrumentIndex = desiredIntrumentIndex;
        // }
    }
    // TODO : TODO
    else if (actionType == "UPDATESELECTEDINSTRUMENTSAMPLE")
    {
        int desiredSampleFileIndex = stoi(actionParameters);
        // Clear memory from previous sample
        freeFile(statePointer->instruments[statePointer->currentPartInstrumentIndex].buffer);

        bool loadInstrumentRes = loadInstrument(statePointer->wavFilePaths[desiredSampleFileIndex], true, 0.5, 0, &statePointer->instruments[statePointer->currentPartInstrumentIndex]);
        if (!loadInstrumentRes)
        {
            printf("Failed to update file from %s to %s\n", statePointer->instruments[statePointer->currentPartInstrumentIndex].sample.filePath, statePointer->wavFilePaths[desiredSampleFileIndex]);
        }
    }
    // TODO : FUSIONNER COMME DANS LE FRONT
    else if (actionType == "UPDATEINSTRUMENTSAMPLEVOLUME")
    {
        statePointer->instruments[statePointer->currentPartInstrumentIndex].volume = std::stof(actionParameters);
    }
    // TODO : FUSIONNER COMME DANS LE FRONT
    else if (actionType == "UPDATEINSTRUMENTSAMPLESTEPVOLUME")
    {
        printf("UPDATEINSTRUMENTSAMPLESTEPVOLUME\n");
        int xxxIndex = 0;
        for (int i = 0; i < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); i++)
        {
            int stepInstrumentIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].instrumentIndex;
            if (stepInstrumentIndex == statePointer->currentPartInstrumentIndex)
            {
                xxxIndex = i;
            }
        }
        statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex + STATE_PART_STEPS_LENGTH * statePointer->currentStaveIndex][xxxIndex].volume = std::stof(actionParameters);
    }
    // TODO : FUSIONNER COMME DANS LE FRONT
    else if (actionType == "UPDATEINSTRUMENTSAMPLEPITCH")
    {
        statePointer->instruments[statePointer->currentPartInstrumentIndex].pitch = std::stof(actionParameters);
    }
    // TODO : FUSIONNER COMME DANS LE FRONT
    else if (actionType == "UPDATEINSTRUMENTSAMPLESTEPPITCH")
    {
        printf("UPDATEINSTRUMENTSAMPLESTEPPITCH\n");
        int xxxIndex = 0;
        for (int i = 0; i < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); i++)
        {
            int stepInstrumentIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].instrumentIndex;
            if (stepInstrumentIndex == statePointer->currentPartInstrumentIndex)
            {
                xxxIndex = i;
            }
        }
        statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex + STATE_PART_STEPS_LENGTH * statePointer->currentStaveIndex][xxxIndex].pitch = std::stof(actionParameters);
    }
    else if (actionType == "TOGGLEINSTRUMENTSTEP")
    {
        const int stepIndex = stoi(actionParameters);

        bool isDrumRackSampleStepActive = false;
        for (int stepContentIndex = 0; stepContentIndex < statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].size(); stepContentIndex++)
        {
            if (statePointer->parts[statePointer->currentPartIndex].steps[stepIndex][stepContentIndex].instrumentIndex == statePointer->currentPartInstrumentIndex)
            {
                std::vector<Step>::iterator it = statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].begin() + stepContentIndex;
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
    return ESP_OK;
}
httpd_uri_t action_uri = {
    .uri = "/action",
    .method = HTTP_POST,
    .handler = action_handler,
    .user_ctx = NULL};

bool initWebServer(State *statePointer_p, httpd_handle_t *serverPointer_p, esp_netif_t *netif)
{
    serverPointer = serverPointer_p;
    statePointer = statePointer_p;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.stack_size = 20480; // TODO : test random values

    esp_err_t httpd_start_ret = httpd_start(serverPointer_p, &config);
    if (httpd_start_ret != ESP_OK)
    {
        printf("Failed to start HTTP server\n");
        return false;
    }

    esp_err_t httpd_register_uri_handler_ret = httpd_register_uri_handler(*serverPointer_p, &root_uri);
    if (httpd_register_uri_handler_ret != ESP_OK)
    {
        printf("Failed to register root uri handler\n");
        return false;
    }
    httpd_register_uri_handler_ret = httpd_register_uri_handler(*serverPointer_p, &websocket_uri);
    if (httpd_register_uri_handler_ret != ESP_OK)
    {
        printf("Failed to register /ws uri handler\n");
        return false;
    }
    httpd_register_uri_handler_ret = httpd_register_uri_handler(*serverPointer_p, &state_uri);
    if (httpd_register_uri_handler_ret != ESP_OK)
    {
        printf("Failed to register /state uri handler\n");
        return false;
    }
    httpd_register_uri_handler_ret = httpd_register_uri_handler(*serverPointer_p, &wavFiles_uri);
    if (httpd_register_uri_handler_ret != ESP_OK)
    {
        printf("Failed to register /wavFiles uri handler\n");
        return false;
    }
    httpd_register_uri_handler_ret = httpd_register_uri_handler(*serverPointer_p, &action_uri);
    if (httpd_register_uri_handler_ret != ESP_OK)
    {
        printf("Failed to register /action uri handler\n");
        return false;
    }

    return true;
}
