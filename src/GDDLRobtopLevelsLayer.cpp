#include "GDDLRobtopLevelsLayer.h"

#include <GDDLBoomScrollLayer.h>

bool GDDLRobtopLevelsLayer::init(int page) {
    if (!LevelSelectLayer::init(page)) {
        return false;
    }
    m_fields->beingBrowsed = true;
    GDDLBoomScrollLayer::Fields::robtopLevelsLayer = this;
    // setup potential web req
    m_fields->robtopLevelsLayerGetRatingListener.bind([this](web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            const std::string response = res->string().unwrapOrDefault();
            if (response.empty()) {
                updateButton(-1);
            }
            else {
                const int levelID = convertPageToLevel(m_fields->currentPage);
                int tierAfterFetch = -1;
                if (RatingsManager::addRatingFromResponse(levelID, response)) {
                    tierAfterFetch = RatingsManager::getDemonTier(levelID);
                }
                updateButton(tierAfterFetch);
            }
        }
        else if (e->isCancelled()) {
            updateButton(-1);
        }
    });

    m_fields->currentPage = page;
    pageChanged(-1);
    return true;
}

void GDDLRobtopLevelsLayer::onNext(CCObject* sender) {
    int previousPage = m_fields->currentPage++;
    if (m_fields->currentPage >= m_fields->pageCount) {
        m_fields->currentPage = 0;
    }
    pageChanged(previousPage);
    LevelSelectLayer::onNext(sender);
}

void GDDLRobtopLevelsLayer::onPrev(CCObject* sender) {
    int previousPage = m_fields->currentPage--;
    if (m_fields->currentPage < 0) {
        m_fields->currentPage = m_fields->pageCount - 1;
    }
    pageChanged(previousPage);
    LevelSelectLayer::onPrev(sender);
}

void GDDLRobtopLevelsLayer::swiped(const int newPage) {
    if (newPage == m_fields->currentPage) return;
    m_fields->changedBySwiping = true;
    const int previousPage = m_fields->currentPage;
    m_fields->currentPage = newPage;
    pageChanged(previousPage);
}

void GDDLRobtopLevelsLayer::onBack(CCObject* sender) {
    LevelSelectLayer::onBack(sender);
    backActions();
}

void GDDLRobtopLevelsLayer::keyBackClicked() {
    LevelSelectLayer::keyBackClicked();
    backActions();
}

void GDDLRobtopLevelsLayer::backActions() {
    m_fields->beingBrowsed = false;
    GDDLBoomScrollLayer::Fields::robtopLevelsLayer = nullptr;
}

void GDDLRobtopLevelsLayer::pageChanged(int previousPage) {
    // first, remove any buttons that should be removed
    if (m_fields->currentPage == Level::TOE && previousPage == Level::ELECTROMAN_ADVENTURES &&
        m_fields->buttonsAdded[0]) {
        // navigated away from clubstep to the left, button should be removed
        removeFrom(2);
        m_fields->buttonsAdded[0] = false;
    }
    else if (m_fields->currentPage == Level::HEXAGON_FORCE && previousPage == Level::ELECTRODYNAMIX &&
        m_fields->buttonsAdded[0]) {
        // navigated away from clubstep to the right, button should be removed
        removeFrom(2);
        m_fields->buttonsAdded[0] = false;
    }
    else if (m_fields->currentPage == Level::HEXAGON_FORCE && previousPage == Level::BLAST_PROCESSING &&
        m_fields->buttonsAdded[1]) {
        // navigated away from toe2 to the left, button should be removed
        removeFrom(3);
        m_fields->buttonsAdded[1] = false;
    }
    else if (m_fields->currentPage == Level::DEADLOCKED && previousPage == Level::GEOMETRICAL_DOMINATOR &&
        m_fields->buttonsAdded[1]) {
        // navigated away from toe2 to the right, button should be removed
        removeFrom(3);
        m_fields->buttonsAdded[1] = false;
    }
    else if (m_fields->currentPage == Level::TOE2 && previousPage == Level::GEOMETRICAL_DOMINATOR &&
        m_fields->buttonsAdded[2]) {
        // navigated away from deadlocked to the left, button should be removed
        removeFrom(2);
        m_fields->buttonsAdded[2] = false;
    }
    else if (m_fields->currentPage == Level::DASH && previousPage == Level::FINGERDASH &&
        m_fields->buttonsAdded[2]) {
        // navigated away from deadlocked to the right, button should be removed
        removeFrom(2);
        m_fields->buttonsAdded[2] = false;
    }
    // then add them if that's required
    if (m_fields->currentPage == Level::CLUBSTEP && !m_fields->buttonsAdded[0]) {
        // just navigated to clubstep
        addTo(2, 1);
        m_fields->buttonsAdded[0] = true;
    }
    else if (m_fields->currentPage == Level::TOE2 && !m_fields->buttonsAdded[1]) {
        // just navigated to toe2
        addTo(3, 2);
        m_fields->buttonsAdded[1] = true;
    }
    else if (m_fields->currentPage == Level::DEADLOCKED && !m_fields->buttonsAdded[2]) {
        // just navigated to deadlocked
        addTo(2, 3);
        m_fields->buttonsAdded[2] = true;
    }
}

void GDDLRobtopLevelsLayer::removeFrom(int scrollLayerPage) {
    if (m_fields->changedBySwiping) {
        // for some reason the usual way of doing that bugs out in that case
        // find the button menu on all layers and remove them
        auto buttonMenu = getChildByIDRecursive("gddl-button-menu"_spr);
        while (buttonMenu != nullptr) {
            auto buttonMenuParent = buttonMenu->getParent();
            buttonMenuParent->removeChildByID("gddl-button-menu"_spr);
            buttonMenu = getChildByIDRecursive("gddl-button-menu"_spr);
        }
        // same for the button because sometimes it decides to reattach itself to the nearest random menu????
        auto button = getChildByIDRecursive("gddl-button"_spr);
        while (button != nullptr) {
            auto buttonParent = button->getParent();
            buttonParent->removeChildByID("gddl-button"_spr);
            button = getChildByIDRecursive("gddl-button"_spr);
        }
        m_fields->changedBySwiping = false;
    } else {
        const std::string pageID = "level-page-" + std::to_string(scrollLayerPage);
        auto levelButton = getLevelButton(scrollLayerPage);
        levelButton->removeChildByID("gddl-button-menu"_spr);
    }
}

void GDDLRobtopLevelsLayer::addTo(int scrollLayerPage, int levelID) {
    // create the buttonMenu and the button
    if (!Utils::notExcluded(levelID)) {
        return;
    }
    const auto buttonMenu = CCMenu::create();
    buttonMenu->setID("gddl-button-menu"_spr);
    buttonMenu->setContentSize({25, 25});
    const auto sprite = Utils::getSpriteFromTier(RatingsManager::getCachedTier(levelID));
    sprite->setScale(sprite->getScale() / 2);
    const auto button = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(GDDLRobtopLevelsLayer::onGDDLInfo));
    button->setID("gddl-button"_spr);
    // add it
    auto levelButton = getLevelButton(scrollLayerPage);
    if (!levelButton) return;
    buttonMenu->addChild(button);
    button->setPosition(12.5, 12.5);
    levelButton->addChild(buttonMenu);
    buttonMenu->setPosition(5, 69);
    // after placing the button fetch the full level info
    const int tier = RatingsManager::getDemonTier(levelID);
    if (tier == -1) {
        auto req = web::WebRequest();
        m_fields->robtopLevelsLayerGetRatingListener.setFilter(req.get(RatingsManager::getRequestUrl(levelID)));
    }
}

int GDDLRobtopLevelsLayer::convertPageToLevel(int page) {
    int levelID = 1;
    if (page == Level::TOE2) {
        levelID = 2;
    } else if (page == Level::DEADLOCKED) {
        levelID = 3;
    }
    return levelID;
}

CCNode* GDDLRobtopLevelsLayer::getLevelButton(int pageID) {
    const std::string pageIDStr = "level-page-" + std::to_string(pageID);
    return getChildByID("levels-list")->getChildByID("level-pages")->getChildByID(pageIDStr)->getChildByID(
            "level-menu")->getChildByID("level-button")->getChildByID("white-sprite")->getChildByID(
            "scale-9-sprite");
}

void GDDLRobtopLevelsLayer::onGDDLInfo(CCObject *sender) {
    const int levelID = convertPageToLevel(m_fields->currentPage);
    GDDLLevelInfoPopup::create(levelID)->show();
}

void GDDLRobtopLevelsLayer::updateButton(const int tier) {
    // get the menu
    auto gddlButtonMenu = getLevelButton(m_fields->currentPage % 3 + 1)->getChildByID("gddl-button-menu"_spr);
    if (gddlButtonMenu == nullptr) return; // the user most likely scrolled past quicker than it loaded
    // remove the old button
    gddlButtonMenu->removeAllChildren();
    // add the new one
    const int levelID = convertPageToLevel(m_fields->currentPage);
    const auto sprite = Utils::getSpriteFromTier(RatingsManager::getCachedTier(levelID));
    sprite->setScale(sprite->getScale() / 2);
    const auto button = CCMenuItemSpriteExtra::create(sprite, this,menu_selector(GDDLRobtopLevelsLayer::onGDDLInfo));
    button->setID("gddl-button"_spr);
    gddlButtonMenu->addChild(button);
    button->setPosition(12.5, 12.5);
}