function [patch_y, patch_x] = findPatch( patch, area, MSE_THRESHOLD )
  [patch_size_x, patch_size_y] = size( patch );
  [area_size_x, area_size_y] = size( area );
  center_patch_y = patch_size_y + 1;
  center_patch_x = patch_size_x + 1;
  search_patch = area(center_patch_y : center_patch_y + patch_size_y - 1,...
                      center_patch_x : center_patch_x + patch_size_x - 1);
  curMSE = mse2D( patch, search_patch );
  minMSE = curMSE;
  patch_y = center_patch_y;
  patch_x = center_patch_x;
  for y = 1 : 1 : area_size_y - patch_size_y + 1
    for x = 1 : 1 : area_size_x - patch_size_x + 1
      search_patch = area(y : y + patch_size_y - 1, x : x + patch_size_x - 1);
      curMSE = mse2D( patch, search_patch );
      if curMSE < ( minMSE - MSE_THRESHOLD )
        minMSE = curMSE;
        patch_y = y;
        patch_x = x;
      end
    end
  end
end