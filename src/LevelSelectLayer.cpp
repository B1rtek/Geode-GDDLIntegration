#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>

class $modify(GDDLRobtopLevelsLayer, LevelSelectLayer) {

    enum Level {
        TOE = 11,
        ELECTROMAN_ADVENTURES = 12,
        CLUBSTEP = 13,
        ELECTRODYNAMIX = 14,
        HEXAGON_FORCE = 15,
        BLAST_PROCESSING = 16,
        TOE2 = 17,
        GEOMETRICAL_DOMINATOR = 18,
        DEADLOCKED = 19,
        FINGERDASH = 20,
        DASH = 21,
    };

    struct Fields {
        int currentPage = 0;
        const static int pageCount = 24;
        bool buttonsAdded[3] = {false, false, false};
    };

    bool init(int page) {
        if (!LevelSelectLayer::init(page)) {
            return false;
        }
        m_fields->currentPage = page;
        pageChanged(-1);
        return true;
    }

    void onNext(CCObject* sender) {
        LevelSelectLayer::onNext(sender);
        int previousPage = m_fields->currentPage++;
        if (m_fields->currentPage >= m_fields->pageCount) {
            m_fields->currentPage = 0;
        }
        pageChanged(previousPage);
    }

    void onPrev(CCObject* sender) {
        LevelSelectLayer::onPrev(sender);
        int previousPage = m_fields->currentPage--;
        if (m_fields->currentPage < 0) {
            m_fields->currentPage = m_fields->pageCount - 1;
        }
        pageChanged(previousPage);
    }

    void pageChanged(int previousPage) {
        if (m_fields->currentPage == Level::CLUBSTEP && !m_fields->buttonsAdded[0]) {
            // just navigated to clubstep
            addTo(2, 1);
            m_fields->buttonsAdded[0] = true;
        } else if (m_fields->currentPage == Level::TOE2 && !m_fields->buttonsAdded[1]) {
            // just navigated to toe2
            addTo(3, 2);
            m_fields->buttonsAdded[1] = true;
        } else if (m_fields->currentPage == Level::DEADLOCKED && !m_fields->buttonsAdded[2]) {
            // just navigated to deadlocked
            addTo(2, 3);
            m_fields->buttonsAdded[2] = true;
        }
        // a button might also be removed at the same time
        if (m_fields->currentPage == Level::TOE && previousPage == Level::ELECTROMAN_ADVENTURES && m_fields->buttonsAdded[0]) {
            // navigated away from clubstep to the left, button should be removed
            removeFrom(2);
            m_fields->buttonsAdded[0] = false;
        } else if (m_fields->currentPage == Level::HEXAGON_FORCE && previousPage == Level::ELECTRODYNAMIX && m_fields->buttonsAdded[0]) {
            // navigated away from clubstep to the right, button should be removed
            removeFrom(2);
            m_fields->buttonsAdded[0] = false;
        } else if (m_fields->currentPage == Level::HEXAGON_FORCE && previousPage == Level::BLAST_PROCESSING && m_fields->buttonsAdded[1]) {
            // navigated away from toe2 to the left, button should be removed
            removeFrom(3);
            m_fields->buttonsAdded[1] = false;
        } else if (m_fields->currentPage == Level::DEADLOCKED && previousPage == Level::GEOMETRICAL_DOMINATOR && m_fields->buttonsAdded[1]) {
            // navigated away from toe2 to the right, button should be removed
            removeFrom(3);
            m_fields->buttonsAdded[1] = false;
        } else if (m_fields->currentPage == Level::TOE2 && previousPage == Level::GEOMETRICAL_DOMINATOR && m_fields->buttonsAdded[2]) {
            // navigated away from deadlocked to the left, button should be removed
            removeFrom(2);
            m_fields->buttonsAdded[2] = false;
        } else if (m_fields->currentPage == Level::DASH && previousPage == Level::FINGERDASH && m_fields->buttonsAdded[2]) {
            // navigated away from deadlocked to the right, button should be removed
            removeFrom(2);
            m_fields->buttonsAdded[2] = false;
        }
    }

    void removeFrom(int scrollLayerPage) {
        std::cout << "[gddlint] remove from page " << scrollLayerPage << std::endl;
    }

    void addTo(int scrollLayerPage, int levelID) {
        std::cout << "[gddlint] add to page " << scrollLayerPage << " level " << levelID << std::endl;
    }
};