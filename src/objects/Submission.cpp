#include "Submission.h"

#include <Utils.h>

/**
 * request
 * {
 *  "levelID":341613,
 *  "rating":2,
 *  "enjoyment":6,
 *  "refreshRate":60,
 *  "device":1, - 1 for pc, 2 for mobile
 *  "proof":"https://youtu.be/3-BUEoH9WBs",
 *  "progress":100,
 *  "attempts":403,
 *  "isSolo":true - default for both 1p and 2p levels
 *  "secondPlayerID": int | null
 *  }
 *
 *  response
 *  {
 *   "LevelID":341613,
 *   "Rating":2,
 *   "Enjoyment":6,
 *   "RefreshRate":60,
 *   "Device":"Mobile",
 *   "Proof": null,
 *   "IsSolo":true,
 *   "SecondPlayerID: null,
 *   "Progress": 100,
 *   "Attempts": 69,
 *  }
 */

Submission::Submission(matjson::Value json, bool request) {
    // request and response jsons are slightly different, idk why
    if (request) {
        this->levelID = json["levelID"].asInt().unwrap();
        this->rating = json.contains("rating") ? json["rating"].asInt().unwrap() : 0;
        this->enjoyment = json.contains("enjoyment") ? json["enjoyment"].asInt().unwrap() : -1;
        this->refreshRate = json.contains("refreshRate") ? json["refreshRate"].asInt().unwrap() : -1;
        this->device = json.contains("device") ? (time(nullptr) < Utils::API_SWITCH_TIME ? json["device"].asInt().unwrap() : json["device"].asString().unwrap() == "pc" ? 1 : 2) : 1;
        this->proof = json.contains("proof") ? json["proof"].asString().unwrap() : "";
        this->progress = json["progress"].asInt().unwrap();
        this->attempts = json.contains("attempts") ? json["attempts"].asInt().unwrap() : -1;
        this->isSolo = json.contains("isSolo") ? json["isSolo"].asBool().unwrap() : true;
        this->secondPlayerID = !this->isSolo ? json["secondPlayerID"].asInt().unwrap() : -1;
    } else {
        this->levelID = json["LevelID"].asInt().unwrap();
        this->rating = !json["Rating"].isNull() ? json["Rating"].asInt().unwrap() : 0;
        this->enjoyment = !json["Enjoyment"].isNull() ? json["Enjoyment"].asInt().unwrap() : -1;
        this->refreshRate = !json["RefreshRate"].isNull() ? json["RefreshRate"].asInt().unwrap() : -1;
        this->device = !json["Device"].isNull() ? (json["Device"].asString().unwrap() == "pc" ? 1 : 2) : 1;
        this->proof = !json["Proof"].isNull() ? json["Proof"].asString().unwrap() : "";
        this->isSolo = !json["IsSolo"].isNull() ? json["IsSolo"].asBool().unwrap() : true;
        this->secondPlayerID = !json["SecondPlayerID"].isNull() ? json["SecondPlayerID"].asInt().unwrap() : -1;
        this->progress = !json["Progress"].isNull() ? json["Progress"].asInt().unwrap() : -1;
        this->attempts = !json["Attempts"].isNull() ? json["Attempts"].asInt().unwrap() : -1;
    }
}

bool Submission::isEmpty() const {
    return this->progress == -1;
}

std::string Submission::describe() const {
    std::string description;
    if (this->rating != 0) {
        description += "Tier <cr>" + std::to_string(this->rating) + "</c>, ";
    } else {
        description += "No rating, ";
    }
    if (this->enjoyment != -1) {
        description += "enjoyment <cg>" + std::to_string(this->enjoyment) + "</c>, ";
    } else {
        description += "no enjoyment, ";
    }
    std::string deviceStr = this->device == 1 ? "PC" : "mobile";
    description += "achieved on <cb>" + deviceStr + "</c>";
    if (this->refreshRate != -1) {
        description += " on <cb>" + std::to_string(this->refreshRate) + "</c> fps, ";
    } else {
        description += ", ";
    }
    description += "progress <co>" + std::to_string(this->progress) + "%</c>";
    if (this->attempts != -1) {
        description += " in <cp>" + std::to_string(this->attempts) + "</c> attempts";
    }
    description += ".";
    return description;
}
