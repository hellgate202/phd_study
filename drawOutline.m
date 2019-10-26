function output = drawOutline( patch, color )
  patch_size = size( patch );
  patch_size_y = patch_size(1);
  patch_size_x = patch_size(2);
  for y = 1 : 1 : patch_size_y
    patch(y,1,:)   = color;
    patch(y,end,:) = color;
  end
  for x = 1 : 1 : patch_size_x
    patch(1,x,:)   = color;
    patch(end,x,:) = color;
  end
  output = patch;
end