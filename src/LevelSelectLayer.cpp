#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>

class $modify(GDDLRobtopLevelsLayer, LevelSelectLayer) {

    struct Fields {
        int currentPage = 0;
        const static int pageCount = 24;
    };

    bool init(int page) {
        if (!LevelSelectLayer::init(page)) {
            return false;
        }
        m_fields->currentPage = page;
        addButton();
        return true;
    }

    void onNext(CCObject* sender) {
        LevelSelectLayer::onNext(sender);
        ++m_fields->currentPage;
        if (m_fields->currentPage >= m_fields->pageCount) {
            m_fields->currentPage = 0;
        }
        addButton();
    }

    void onPrev(CCObject* sender) {
        LevelSelectLayer::onPrev(sender);
        --m_fields->currentPage;
        if (m_fields->currentPage < 0) {
            m_fields->currentPage = m_fields->pageCount - 1;
        }
        addButton();
    }

    void addButton() {
        if (m_fields->currentPage == 13) {
            std::cout << "[gddlint] clubstep" << std::endl;
        } else if (m_fields->currentPage == 17) {
            std::cout << "[gddlint] toe2" << std::endl;
        } else if (m_fields->currentPage == 19) {
            std::cout << "[gddlint] deadlocked" << std::endl;
        }
    }
};