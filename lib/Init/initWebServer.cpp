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

static esp_err_t trigger_async_send(httpd_handle_t handle, httpd_req_t *req, const char *message)
{
    struct async_resp_arg *resp_arg = (async_resp_arg *)malloc(sizeof(struct async_resp_arg));
    resp_arg->hd = req->handle;
    // resp_arg->fd = httpd_req_to_sockfd(req);

    // Copy message into heap so it survives async context
    resp_arg->message = strdup(message);
    if (!resp_arg->message)
    {
        free(resp_arg->message);
        free(resp_arg);
        return ESP_ERR_NO_MEM;
    }
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
                printf("stof(actionParameters) : %lf\n", stof(actionParameters));
                statePointer->masterGain = stof(actionParameters);
            }
            else if (actionType == "UPDATECURRENTMODE")
            {
                statePointer->currentModeIndex = stoi(actionParameters);
            }
            else if (actionType == "UPDATECURRENTSELECTEDSTEP")
            {
                statePointer->currentSelectedStepIndex = stoi(actionParameters);
            }

            else if (actionType == "CREATESONGATINDEX")
            {
                int songIndex = stoi(actionParameters);
                createSongAtIndex(statePointer, songIndex);
                statePointer->currentSongIndex = songIndex;
                readSong(statePointer, songIndex);
            }
            else if (actionType == "DELETESONGATINDEX")
            {
                int songIndex = stoi(actionParameters);
                deleteSongAtIndex(statePointer, songIndex);
                statePointer->currentSongIndex = songIndex > 0 ? songIndex - 1 : 0;
                readSong(statePointer, songIndex);
            }
            else if (actionType == "UPDATECURRENTSONGINDEX")
            {
                int songIndex = stoi(actionParameters);
                readSong(statePointer, songIndex);
            }
            else if (actionType == "UPDATESONGNAME")
            {
                // char *copy;
                // strcpy(copy, actionParameters.c_str());
                // statePointer->songName = copy;
                strcpy(statePointer->songName, actionParameters.c_str());
                // statePointer->songName = actionParameters.c_str();
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
                // for (int i = 0; i < statePointer->instruments.size(); i++)
                // {
                //     // if(statePointer->instruments[i] == i) {
                //     //     // TODO
                //     //     // statePointer->samples[statePointer->instruments[i].sampleFileRefIndex].
                //     // } else {

                //     // }
                // }
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
            // TODO : Rename to UPDATESELECTEDSTAVE
            // TODO : Rename others containing "CURRENT" to "SELECTED"
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

                // TODO : Error handling
                // bool loadInstrumentRes =
                loadInstrument(statePointer->wavFilePaths[desiredSampleFileIndex], true, statePointer->instruments[statePointer->currentPartInstrumentIndex].volume, statePointer->instruments[statePointer->currentPartInstrumentIndex].pitch, statePointer->instruments[statePointer->currentPartInstrumentIndex].startPosition, statePointer->instruments[statePointer->currentPartInstrumentIndex].endPosition, statePointer->instruments[statePointer->currentPartInstrumentIndex].isReverse, &statePointer->instruments[statePointer->currentPartInstrumentIndex]);
                // if (!loadInstrumentRes)
                // {
                //     printf("Failed to update file from %s to %s\n", statePointer->instruments[statePointer->currentPartInstrumentIndex].sample.filePath, statePointer->wavFilePaths[desiredSampleFileIndex]);
                // }
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
                statePointer->instruments[statePointer->currentPartInstrumentIndex].pitch = stoi(actionParameters);
            }
            // TODO : FUSIONNER COMME DANS LE FRONT
            else if (actionType == "UPDATEINSTRUMENTSAMPLESTEPPITCH")
            {
                printf("UPDATEINSTRUMENTSAMPLESTEPPITCH\n");
                int xxxIndex = 0;
                for (int i = 0; i < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); i++)
                {
                    // int stepInstrumentIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].instrumentIndex;
                    if (statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].instrumentIndex == statePointer->currentPartInstrumentIndex)
                    {
                        xxxIndex = i;
                    }
                }
                printf("stoi(actionParameters) : %i\n", stoi(actionParameters));
                // statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex + STATE_PART_STEPS_LENGTH * statePointer->currentStaveIndex][xxxIndex].pitch = stoi(actionParameters);

                printf("pitch BEFORE : %i\n", statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex][xxxIndex].pitch);
                statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex][xxxIndex].pitch = stoi(actionParameters);
                printf("pitch AFTER : %i\n", statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex][xxxIndex].pitch);
            }
            // TODO : FUSIONNER COMME DANS LE FRONT
            else if (actionType == "UPDATEINSTRUMENTSAMPLESTARTPOSITION")
            {
                statePointer->instruments[statePointer->currentPartInstrumentIndex].startPosition = std::stof(actionParameters);
            }
            // TODO : FUSIONNER COMME DANS LE FRONT
            else if (actionType == "UPDATEINSTRUMENTSAMPLESTEPSTARTPOSITION")
            {
                printf("UPDATEINSTRUMENTSAMPLESTEPSTARTPOSITION\n");
                int xxxIndex = 0;
                for (int i = 0; i < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); i++)
                {
                    int stepInstrumentIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].instrumentIndex;
                    if (stepInstrumentIndex == statePointer->currentPartInstrumentIndex)
                    {
                        xxxIndex = i;
                    }
                }
                statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex + STATE_PART_STEPS_LENGTH * statePointer->currentStaveIndex][xxxIndex].startPosition = std::stof(actionParameters);
            }
            // TODO : FUSIONNER COMME DANS LE FRONT
            else if (actionType == "UPDATEINSTRUMENTSAMPLEENDPOSITION")
            {
                statePointer->instruments[statePointer->currentPartInstrumentIndex].endPosition = std::stof(actionParameters);
            }
            // TODO : FUSIONNER COMME DANS LE FRONT
            else if (actionType == "UPDATEINSTRUMENTSAMPLESTEPENDPOSITION")
            {
                printf("UPDATEINSTRUMENTSAMPLESTEPENDPOSITION\n");
                int xxxIndex = 0;
                for (int i = 0; i < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); i++)
                {
                    int stepInstrumentIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].instrumentIndex;
                    if (stepInstrumentIndex == statePointer->currentPartInstrumentIndex)
                    {
                        xxxIndex = i;
                    }
                }
                statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex + STATE_PART_STEPS_LENGTH * statePointer->currentStaveIndex][xxxIndex].endPosition = std::stof(actionParameters);
            }
            // TODO : FUSIONNER COMME DANS LE FRONT
            else if (actionType == "UPDATEINSTRUMENTSAMPLEISREVERSE")
            {
                statePointer->instruments[statePointer->currentPartInstrumentIndex].isReverse = std::stof(actionParameters);
            }
            // TODO : FUSIONNER COMME DANS LE FRONT
            else if (actionType == "UPDATEINSTRUMENTSAMPLESTEPISREVERSE")
            {
                printf("UPDATEINSTRUMENTSAMPLESTEPISREVERSE\n");
                int xxxIndex = 0;
                for (int i = 0; i < statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex].size(); i++)
                {
                    int stepInstrumentIndex = statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentStepIndex][i].instrumentIndex;
                    if (stepInstrumentIndex == statePointer->currentPartInstrumentIndex)
                    {
                        xxxIndex = i;
                    }
                }
                statePointer->parts[statePointer->currentPartIndex].steps[statePointer->currentSelectedStepIndex + STATE_PART_STEPS_LENGTH * statePointer->currentStaveIndex][xxxIndex].isReverse = stoi(actionParameters);
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
                    statePointer->parts[statePointer->currentPartIndex].steps[stepIndex].push_back({statePointer->currentPartInstrumentIndex, 1.0, 0, 0.0, 1.0, false});
                }
            }
            else
            {
                printf("Unknown message type\n");
            }

            // Send a response back to the client
            trigger_async_send(req->handle, req, (const char *)ws_pkt.payload);
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
