local path = Resources.GetAssetsPath().."/Scripts/?.lua"
package.path = package.path..";"..path

require("Utils")
require("GameLogicMgr")

fprint("Lua init, version: %s", _VERSION)
