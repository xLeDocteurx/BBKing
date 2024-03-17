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

State *statePointer;

static esp_err_t root_handler(httpd_req_t *req)
{
    // beginning of the ws URI handler
    if (req->method == HTTP_GET)
    {
        // action before ws handshake
        std::string htmlString;
        bool readJsonFile_ret = readJsonFile("/data/index.html", &htmlString);
        // httpd_resp_set_status(req, "200");
        httpd_resp_set_type(req, "text/html");
        // httpd_resp_set_hdr();
        httpd_resp_send(req, htmlString.c_str(), HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }
    // action after the handshake (read frames)
    return ESP_OK;
}
httpd_uri_t root_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = root_handler,
    .user_ctx = NULL,

    .is_websocket = true,              // Mandatory: set to `true` to handler websocket protocol
    .handle_ws_control_frames = false, // Optional: set to `true` for the handler to receive control packets, too
    .supported_subprotocol = "chat",   // Optional: set supported subprotocol for this handler
};

static esp_err_t state_handler(httpd_req_t *req)
{
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
                stepString += std::to_string(statePointer->parts[i].steps[j][k]);
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
        "{\"currentSongIndex\":" + std::to_string(statePointer->currentSongIndex) + ",\"songName\":\"" + statePointer->songName + "\",\"songTempo\":" + std::to_string(statePointer->songTempo) + ",\"currentPartIndex\":" + std::to_string(statePointer->currentPartIndex) + ",\"currentPartInstrument\":" + std::to_string(statePointer->currentPartInstrument) + ",\"parts\":" + partsString + "}";
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
    if (actionType == "PLAYFROMSTART")
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
        statePointer->currentPartInstrument = stoi(actionParameters);
    }
    else if (actionType == "TOGGLEINSTRUMENTSTEP")
    {
        const int stepIndex = stoi(actionParameters);
        statePointer->parts[statePointer->currentPartIndex].steps[stepIndex];

        bool isDrumRackSampleStepActive = false;
        for (int i = 0; i < statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].size(); i++)
        {
            if (statePointer->parts[statePointer->currentPartIndex].steps[stepIndex][i] == statePointer->currentPartInstrument)
            {
                isDrumRackSampleStepActive = true;

                // Find the position of the number 3
                std::vector<int>::const_iterator it = std::find(statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].begin(), statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].end(), statePointer->currentPartInstrument);

                // If the number 3 is found, remove it
                if (it != statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].end())
                {
                    statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].erase(it);
                }
                break;
            }
        }

        if (!isDrumRackSampleStepActive)
        {
            statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].push_back(statePointer->currentPartInstrument);
        }
    }
    else if (actionType == "UPDATETEMPO")
    {
        statePointer->songTempo = stoi(actionParameters);
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
    httpd_register_uri_handler_ret = httpd_register_uri_handler(*server, &state_uri);
    if (httpd_register_uri_handler_ret != ESP_OK)
    {
        printf("Failed to register /state uri handler\n");
        return false;
    }
    httpd_register_uri_handler_ret = httpd_register_uri_handler(*server, &action_uri);
    if (httpd_register_uri_handler_ret != ESP_OK)
    {
        printf("Failed to register /state uri handler\n");
        return false;
    }

    return true;
}
