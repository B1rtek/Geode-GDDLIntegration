#include "Submission.h"

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
        this->levelID = json["levelID"].as_int();
        this->rating = json.contains("rating") ? json["rating"].as_int() : 0;
        this->enjoyment = json.contains("enjoyment") ? json["enjoyment"].as_int() : -1;
        this->refreshRate = json.contains("refreshRate") ? json["refreshRate"].as_int() : -1;
        this->device = json["device"].as_int();
        this->proof = json.contains("proof") ? json["proof"].as_string() : "";
        this->progress = json["progress"].as_int();
        this->attempts = json.contains("attempts") ? json["attempts"].as_int() : -1;
        this->isSolo = json.contains("isSolo") ? json["isSolo"].as_bool() : true;
        this->secondPlayerID = !this->isSolo ? json["secondPlayerID"].as_int() : -1;
    } else {
        this->levelID = json["LevelID"].as_int();
        this->rating = !json["Rating"].is_null() ? json["Rating"].as_int() : 0;
        this->enjoyment = !json["Enjoyment"].is_null() ? json["Enjoyment"].as_int() : -1;
        this->refreshRate = !json["RefreshRate"].is_null() ? json["RefreshRate"].as_int() : -1;
        this->device = !json["Device"].is_null() ? (json["Device"].as_string() == "PC" ? 1 : 2) : 1;
        this->proof = !json["Proof"].is_null() ? json["Proof"].as_string() : "";
        this->isSolo = !json["IsSolo"].is_null() ? json["IsSolo"].as_bool() : true;
        this->secondPlayerID = !json["SecondPlayerID"].is_null() ? json["SecondPlayerID"].as_int() : -1;
        this->progress = !json["Progress"].is_null() ? json["Progress"].as_int() : -1;
        this->attempts = !json["Attempts"].is_null() ? json["Attempts"].as_int() : -1;
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
