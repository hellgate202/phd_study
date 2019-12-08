function [patch_y, patch_x] = findPatch( patch, area )
  [patch_size_x, patch_size_y] = size( patch );
  [area_size_x, area_size_y] = size( patch );
  patch_y = 0;
  patch_x = 0;
  center_patch_y = ceil( area_size_y / 2 ) - floor( patch_size_y / 2 );
  center_patch_x = ceil( area_size_x / 2 ) - floor( patch_size_x / 2 );
  search_patch = area(center_patch_y : center_patch_y + patch_size_y - 1,...
                      center_patch_x : center_patch_x + patch_size_x - 1);
  curMSE = mse2D( patch, search_patch );
  minMSE = curMSE;
  for y = 1 : 1 : area_size_y - patch_size_y + 1
    for x = 1 : 1 : area_size_x - patch_size_x + 1
      search_patch = area(y : y + patch_size_y - 1, x : x + patch_size_x - 1);
      curMSE = mse2D( patch, search_patch );
      if( curMSE < minMSE )
        minMSE = curMSE;
        patch_y = y;
        patch_x = x;
      end
    end
  end
end