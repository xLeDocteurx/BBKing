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
#include <cJSON.h>

#include <Defs.h>
#include <MyUtils.h>
#include <Songs.h>
#include <Actions.h>

State *statePointer;

httpd_handle_t *serverPointer;
struct async_resp_arg
{
    httpd_handle_t hd;
    // int fd;
    char *message; // Dynamically allocated message
};

static void ws_async_send(void *arg)
{
    httpd_ws_frame_t ws_pkt;
    struct async_resp_arg *resp_arg = (async_resp_arg *)arg;

    httpd_handle_t hd = resp_arg->hd;

    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.payload = (uint8_t *)resp_arg->message;
    ws_pkt.len = strlen(resp_arg->message);
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    static size_t max_clients = CONFIG_LWIP_MAX_LISTENING_TCP;
    size_t fds = max_clients;
    int client_fds[max_clients];

    esp_err_t ret = httpd_get_client_list(*serverPointer, &fds, client_fds);
    if (ret != ESP_OK)
    {
        // printf("? %d\n", ret);
        free(resp_arg->message);
        free(resp_arg);
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
    free(resp_arg->message);
    free(resp_arg);
}

// static esp_err_t trigger_async_send(httpd_handle_t handle, httpd_req_t *req, const char *message)
// {
//     struct async_resp_arg *resp_arg = (async_resp_arg *)malloc(sizeof(struct async_resp_arg));
//     resp_arg->hd = req->handle;
//     // Copy message into heap so it survives async context
//     resp_arg->message = strdup(message);
//     if (!resp_arg->message)
//     {
//         free(resp_arg->message);
//         free(resp_arg);
//         return ESP_ERR_NO_MEM;
//     }
//     return httpd_queue_work(handle, ws_async_send, resp_arg);
// }

esp_err_t broadcast_ws_message(const char *message)
{
    if (!serverPointer || !*serverPointer)
    {
        printf("Server not initialized\n");
        return ESP_FAIL;
    }

    struct async_resp_arg *resp_arg = (struct async_resp_arg *)malloc(sizeof(struct async_resp_arg));
    if (!resp_arg)
    {
        return ESP_ERR_NO_MEM;
    }

    resp_arg->hd = *serverPointer;
    // Copy message into heap so it survives async context
    resp_arg->message = strdup(message);
    if (!resp_arg->message)
    {
        free(resp_arg->message);
        free(resp_arg);
        return ESP_ERR_NO_MEM;
    }

    return httpd_queue_work(*serverPointer, ws_async_send, resp_arg);
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

    // if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
    //     strcmp((char *)ws_pkt.payload, "toggle") == 0)
    // {
    //     free(buf);
    //     return trigger_async_send(req->handle, req);
    // }
    // else
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strchr((char *)ws_pkt.payload, '@') != NULL) // Vérifie la présence de '@'
    {
        std::string payloadStr((char *)ws_pkt.payload);
        size_t atPos = payloadStr.find('@');

        if (atPos != std::string::npos)
        {
            std::string actionType = payloadStr.substr(0, atPos);
            std::string actionParameters = payloadStr.substr(atPos + 1);

            printf("Action type: %s\n", actionType.c_str());
            printf("Action parameters: %s\n", actionParameters.c_str());

            // TODO : In a separate file to server for other purposes (keyboard, etc...)
            // TODO : Replace if/else by switch ?
            if (actionType == "SAVESONG")
            {
                saveSong(statePointer);
            }
            else if (actionType == "UPDATEMASTERGAIN")
            {
                updateMasterGain(statePointer, actionParameters);
            }
            else if (actionType == "UPDATECURRENTMODE")
            {
                updateCurrentMode(statePointer, actionParameters);
            }
            else if (actionType == "UPDATECURRENTPARTINDEX")
            {
                updateCurrentPartIndex(statePointer, actionParameters);
            }
            else if (actionType == "UPDATECURRENTSTAVEINDEX")
            {
                updateCurrentStaveIndex(statePointer, actionParameters);
            }
            else if (actionType == "UPDATECURRENTOCTAVEINDEX")
            {
                updateCurrentOctaveIndex(statePointer, actionParameters);
            }
            else if (actionType == "UPDATECURRENTSELECTEDSTEP")
            {
                updateCurrentSelectedStep(statePointer, actionParameters);
            }

            else if (actionType == "CREATESONGATINDEX")
            {
                createSongAtIndex(statePointer, stoi(actionParameters));
            }
            else if (actionType == "DELETESONGATINDEX")
            {
                deleteSongAtIndex(statePointer, stoi(actionParameters));
            }
            else if (actionType == "UPDATECURRENTSONGINDEX")
            {
                updateCurrentSongIndex(statePointer, actionParameters);
            }
            else if (actionType == "UPDATESONGNAME")
            {
                updateSongName(statePointer, actionParameters);
            }
            else if (actionType == "UPDATETEMPO")
            {
                updateTempo(statePointer, actionParameters);
            }
            else if (actionType == "PLAYFROMSTART")
            {
                playFromStart(statePointer, actionParameters);
            }
            else if (actionType == "PLAY")
            {
                play(statePointer, actionParameters);
            }
            else if (actionType == "STOP")
            {
                stop(statePointer, actionParameters);
            }

            else if (actionType == "SOLOPART")
            {
                soloPart(statePointer, actionParameters);
            }
            else if (actionType == "MUTEPART")
            {
                mutePart(statePointer, actionParameters);
            }

            else if (actionType == "SELECTPART")
            {
                selectPart(statePointer, actionParameters);
            }
            else if (actionType == "CREATEPART")
            {
                createPart(statePointer, actionParameters);
            }
            // TODO : Rename to UPDATESELECTEDSTAVE
            // TODO : Rename others containing "CURRENT" to "SELECTED"
            else if (actionType == "UPDATESTAVENUMBER")
            {
                updateStaveNumber(statePointer, actionParameters);
            }
            else if (actionType == "SELECTSTAVE")
            {
                selectStave(statePointer, actionParameters);
            }
            else if (actionType == "SELECTOCTAVE")
            {
                selectOctave(statePointer, actionParameters);
            }

            else if (actionType == "SELECTINSTRUMENT")
            {
                selectInstrument(statePointer, actionParameters);
            }
            else if (actionType == "UPDATESELECTEDINSTRUMENTSAMPLE")
            {
                updateSelectedInstrumentSample(statePointer, actionParameters);
            }

            else if (actionType == "UPDATEINSTRUMENTSAMPLEVOLUME")
            {
                updateInstrumentSampleVolume(statePointer, actionParameters);
            }

            else if (actionType == "UPDATEINSTRUMENTSAMPLESTEPVOLUME")
            {
                updateInstrumentSampleStepVolume(statePointer, actionParameters);
            }

            else if (actionType == "UPDATEINSTRUMENTSAMPLEPITCH")
            {
                updateInstrumentSamplePitch(statePointer, actionParameters);
            }

            else if (actionType == "UPDATEINSTRUMENTSAMPLESTEPPITCH")
            {
                updateInstrumentSampleStepPitch(statePointer, actionParameters);
            }

            else if (actionType == "UPDATEINSTRUMENTSAMPLESTARTPOSITION")
            {
                updateInstrumentSampleStartPosition(statePointer, actionParameters);
            }

            else if (actionType == "UPDATEINSTRUMENTSAMPLESTEPSTARTPOSITION")
            {
                updateInstrumentSampleStepStartPosition(statePointer, actionParameters);
            }

            else if (actionType == "UPDATEINSTRUMENTSAMPLEENDPOSITION")
            {
                updateInstrumentSampleEndPosition(statePointer, actionParameters);
            }

            else if (actionType == "UPDATEINSTRUMENTSAMPLESTEPENDPOSITION")
            {
                updateInstrumentSampleStepEndPosition(statePointer, actionParameters);
            }

            else if (actionType == "UPDATEINSTRUMENTSAMPLEISREVERSE")
            {
                updateInstrumentSampleIsReverse(statePointer, actionParameters);
            }

            else if (actionType == "UPDATEINSTRUMENTSAMPLESTEPISREVERSE")
            {
                updateInstrumentSampleStepIsReverse(statePointer, actionParameters);
            }

            else if (actionType == "TOGGLEINSTRUMENTSTEP")
            {
                toggleInstrumentStep(statePointer, actionParameters);
            }
            else
            {
                printf("Unknown message type\n");
                // Send a response back to the client
                broadcast_ws_message((const char *)ws_pkt.payload);
            }
        }

        free(buf);
        return ESP_OK;
    }
    else
    {
        printf("Received unsupported frame type %d\n", ws_pkt.type);
        free(buf);
        return ESP_FAIL;
    }

    free(buf);
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
    // TODO : error handling
    // printf("1\n");
    cJSON *cJsonObject = cJSON_CreateObject();
    // printf("2\n");
    getMachineStateAsCJson(statePointer, cJsonObject);
    // printf("3\n");
    // printf("%s\n", cJSON_Print(cJsonObject));
    // printf("4\n");
    // printf("%s\n", cJsonObject->valuestring);
    // printf("5\n");
    httpd_resp_set_type(req, "text/json");
    // printf("6\n");
    // httpd_resp_send(req, cJsonObject->valuestring, HTTPD_RESP_USE_STRLEN);
    httpd_resp_send(req, cJSON_Print(cJsonObject), HTTPD_RESP_USE_STRLEN);
    // printf("7\n");
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

bool initWebServer(State *statePointer_p, httpd_handle_t *serverPointer_p, esp_netif_t *netif)
{
    serverPointer = serverPointer_p;
    statePointer = statePointer_p;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    // config.stack_size = 20480; // TODO : test random values
    // config.stack_size = 10240; // TODO : test random values
    // config.stack_size = 4096; // TODO : test random values
    // config.stack_size = 8192; // TODO : test random values
    config.stack_size = 12288; // TODO : test random values

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
    // httpd_register_uri_handler_ret = httpd_register_uri_handler(*serverPointer_p, &action_uri);
    // if (httpd_register_uri_handler_ret != ESP_OK)
    // {
    //     printf("Failed to register /action uri handler\n");
    //     return false;
    // }

    return true;
}
