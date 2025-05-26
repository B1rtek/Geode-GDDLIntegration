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
        this->levelID = json["levelID"].asInt().unwrapOr(-1);
        this->rating = json["rating"].asInt().unwrapOr(0);
        this->enjoyment = json["enjoyment"].asInt().unwrapOr(-1);
        this->refreshRate = json["refreshRate"].asInt().unwrapOr(-1);
        this->device = json["device"].asString().unwrapOr("pc") == "pc" ? 1 : 2;
        this->proof = json["proof"].asString().unwrapOr("");
        this->progress = json["progress"].asInt().unwrapOr(-1);
        this->attempts = json["attempts"].asInt().unwrapOr(-1);
        this->isSolo = json["isSolo"].asBool().unwrapOr(true);
        this->secondPlayerID = !this->isSolo ? json["secondPlayerID"].asInt().unwrapOr(-1) : -1;
    } else {
        this->levelID = json["LevelID"].asInt().unwrapOr(-1);
        this->rating = json["Rating"].asInt().unwrapOr(0);
        this->enjoyment = json["Enjoyment"].asInt().unwrapOr(-1);
        this->refreshRate = json["RefreshRate"].asInt().unwrapOr(-1);
        this->device = json["Device"].asString().unwrapOr("pc") == "pc" ? 1 : 2;
        this->proof = json["Proof"].asString().unwrapOr("");
        this->isSolo = json["IsSolo"].asBool().unwrapOr(true);
        this->secondPlayerID = json["SecondPlayerID"].asInt().unwrapOr(-1);
        this->progress = json["Progress"].asInt().unwrapOr(-1);
        this->attempts = json["Attempts"].asInt().unwrapOr(-1);
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
