# v1.1.0
- The search menu has been replaced by a much better one that allows the user to take full advantage of the GDDL API
- Results in the GDDL Demon Split displayed in the level list after clicking a tier are now accurate according to the data in the GDDL API (that's why the numbers might not match, as the counts are generated using local cache)
- The GDDL Search button can be now moved down (look in settings)
- The GDDL Tier button on the level info page can be replaced again with a label, and it can be moved down (although it might overlap with the diamonds label in gauntlets and weeklies)

# v1.0.5
- Added an option to search for levels by tier
- You can find that in the search menu, there's a GDDL button located on the right side

# v1.0.4
- Added an option to move the tier rating button next to the level name (either on the right or the left side), the placement can be controlled with dedicated settings
  - [feature suggestion by thesuperjepphykiller](https://github.com/B1rtek/Geode-GDDLIntegration/issues/6)
- GDDL Demon Split info popup has more information now
- Info showed after clicking the tier button on the level page has <cp>c</c><cr>o</c><co>l</c><cy>o</c><cg>r</c><cj>s</c> now
- The game shouldn't crash anymore after opening the profile page now (added `geode.node-ids` as a dependency)
  - [bug report from Lexicon](https://github.com/B1rtek/Geode-GDDLIntegration/issues/7)

# v1.0.3
Added the *GDDL Demon Split* screen, which shows how many demons of each tier you've beaten (according to a list that gets cached every 7 days)

![And it looks like this](b1rtek.gddlintegration/split.png)