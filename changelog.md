# v1.2.5
- [Fixed a bug which made skillsets disappear](https://github.com/B1rtek/Geode-GDDLIntegration/pull/88) (thank you Diversion!)

# v1.2.4
- Fixed bugs in the API v2 code
- Fixed a bug with the loading circle remaining on the screen after a failed search
- Fixed even more data validation bugs
- Updated error messages to make them more meaningful

# v1.2.3
- Fixed a bug where the GDDL button was placed under the weekly/event demon label
- Added iOS compatibility (I have no way to test this btw, so I can't really help you if anything goes wrong)
- Fixed a major bug in the ratings cache response parsing
- Added compatibility with GDDL API v2 (which will hopefully work correctly, we'll see

# v1.2.1
- [Replaced the "crappy" skillset icons](https://github.com/B1rtek/Geode-GDDLIntegration/issues/72) with a pack created by Hubercioch (thank you so much!!!)
- [Added an option to change the color of the gray backgrounds](https://github.com/B1rtek/Geode-GDDLIntegration/issues/74) in the login, submission and advanced level info popups
- [Fixed a bug that caused crashes on android](https://github.com/B1rtek/Geode-GDDLIntegration/issues/75) if you used the navigation buttons on your phone or a gesture to exit the main levels list
- Fixed a bug that caused a crash sometimes when the rating inside the advanced popup was being updated (thank you @hiimjasmine00!)
- The Mac version is back, enjoy it while it lasts

# v1.2.0
- Added an option to submit ratings from inside the game! Click the GDDL button on the level page to do that (and more!)
  - This requires you to log in to your GDDL account, you can log in on the mod settings page, but if you forget to do that you will be prompted to do so when you try submitting a rating
  - You will be warned if you have already submitted a rating for the level, don't worry about that
- Completely overhauled the popup with rating and enjoyment information that appears after clicking the GDDL button on the level info page
  - Now it shows the top skillsets for the level, rating and enjoyment spreads, allows you to submit the rating for the level and watch its showcase
  - In case you don't like it, you can always turn it off and still use the old one in the mod settings
- Settings have been split into categories, so they aren't as cluttered anymore
- Some popups were replaced with toast messages instead, loading indicators have been added in multiple places
- Fixed the bug that caused sorting in GDDL search to malfunction
- Fixed a bug that caused the GDDL tier button to be placed far away from the level name in certain cases when using the "to the left/right of the level title" setting

MASSIVE THANKS to [dankmeme01](https://github.com/dankmeme01) (main developer of [Globed](https://globed.dev/)) for help with this update! <3

# v1.1.13
- Added difficulty tiers to RobTop's demons
- Ported settings to SettingsV3, making GDDL Integration one of the first mods to use the new standard (yaaaaay!)

# v1.1.12
- Added a GDDL cache reset button in the settings, in case the GDDL Split shows only zeroes you should use it, removing the json file will no longer be needed
- If you're disconnected from the internet the mod should no longer show you an alert about that every time you're in the main menu
- MacOS support (thanks ninXout), although I won't debug issues related only to it because I have no way to do that (that's why it wasn't supported in the first place)

# v1.1.11
- Fixed (hopefully completely and finally) a bug causing the game to crash while browsing newly rated demon levels (thank you hiimjustin000)

# v1.1.10
- Switch to GDDL API 2.0

# v1.1.9
- Fixed a very silly oversight (I accidentally removed a null check), I'm sorry for this :(

# v1.1.8
- 2.206 support
- Settings page overhaul, now it should make a bit more sense
- Fixed the [open in browser button texture](https://github.com/B1rtek/Geode-GDDLIntegration/issues/46)

# v1.1.7
- Addressed a [bug that caused the game to crash when opening the GDDL Search menu](https://github.com/B1rtek/Geode-GDDLIntegration/issues/40)

# v1.1.6
- Added a way to [disable the GDDL button on levels in a specified tier range](https://github.com/B1rtek/Geode-GDDLIntegration/issues/34) (or enable it only in that range)
- The placement of the GDDL Search UI button is now automated and shouldn't collide with other mods thanks to Node IDs 1.9.0
- Fixed a [bug that would prevent the player from placing checkpoints in practice mode due to still being clicked into the default in-game search bar](https://github.com/B1rtek/Geode-GDDLIntegration/issues/33)

# v1.1.5
- Fixed a bug that would cause the game to crash if the level cache was for some reason not a valid json
- The "couldn't cache" popup shouldn't pop up every single time when there's no connection anymore on the title screen
- Fixed a bug which caused the weekly full cache refresh to not work due to overwriting the initial cache time value

# v1.1.4
- Fixed [the bug that prevented players from using the simple search UI](https://github.com/B1rtek/Geode-GDDLIntegration/issues/29)
- The local ratings cache updates downloaded from the API are now saved, so the GDDL Split should be even more accurate now
- Added a button showing completed unrated levels in the GDDL Demon Split screen
- Fixed searching for unrated levels
- Fixed crashes related to putting invalid numbers as tier/enjoyment rating in the full search UI on Android
- Fixed a bug that would cause the search to be called more times than necessary if the search button was clicked twice or more (or to stop working until you reopen it)

# v1.1.3
- Replaced the search interface switching button with a better one
- Added links to GDDL in the mod description and in level details
- The tier search in the simple search UI doesn't overwrite full search interface settings anymore
- Fixed crashes caused by putting invalid tier and enjoyment ratings search criteria
- Possibly fixed the ["typing into a text box that's no longer there" crash](https://github.com/B1rtek/Geode-GDDLIntegration/issues/27)

# v1.1.2
- Going back from search results goes back to the right place (finally!)
- Added a missing alert saying that the rating is still loading (while it's actually loading)
- Updated the search menu to be compatible with the new GDDL API version

# v1.1.1
- The old search menu can be opened with a click on a green arrow on the bottom right of the search popup
- Android users can finally input the correct amount of characters into text fields
- Old tier label displays on top of the epic/legendary/mythic fire
- The tier button/label is given a texture/color and content that matches the tier in the local cache, making it feel like the rating is instantly loaded

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