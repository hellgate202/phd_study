function [class_cov, mean_x, mean_y] = getClassStat( classes, class_num, block_size )
  x_vec = [];
  y_vec = [];
  [y_size, x_size] = size( classes );
  for y = 1 : 1 : y_size
    for x = 1 : 1 : x_size
      if classes(y,x) == class_num
        x_vec = [x_vec ( x - 1 ) * block_size + 1];
        y_vec = [y_vec ( y - 1 ) * block_size + 1];
        x_vec = [x_vec  x * block_size];
        y_vec = [y_vec ( y - 1 ) * block_size + 1];
        x_vec = [x_vec  x * block_size];
        y_vec = [y_vec  y * block_size];
        x_vec = [x_vec ( x - 1 ) * block_size + 1];
        y_vec = [y_vec  y * block_size];
      end
    end
  end
  class_cov = cov( x_vec, y_vec );
  mean_x = mean( x_vec );
  mean_y = mean( y_vec );
end