function output = mse2D( patch_0, patch_1 )
  output = sum( ( patch_0 - patch_1 ) .^ 2, 'all' ) / prod( size( patch_0 ), 'all');
end