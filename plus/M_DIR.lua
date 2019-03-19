local moduleName = ... 
local M = {}
_G[moduleName] = M

function M.Start(ext) 
   local mFiles={}
   local i=1
   for name,size in pairs(file.list()) do
      local mext=name:match "[^.]+$"
      if mext==ext then 
         mFiles[i]={}
         mFiles[i]['name']=name
         mFiles[i]['size']=size
         i=i+1
      end	
   end

   local sort_func = function( a,b ) return a.name < b.name end
   table.sort(mFiles, sort_func)

	return mFiles
end

return M
