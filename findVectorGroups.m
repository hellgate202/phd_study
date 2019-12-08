function obj_map = findVectorGroups( vec_map, d0, search_rad )
  [vec_map_size_y, vec_map_size_x] = size( vec_map );
  obj_map = zeros( size( vec_map ), 'uint8' );
  obj_cnt = 0;
  for y = 2 : 1 : vec_map_size_y - 1
    for x = 2 : 1 : vec_map_size_x - 1
      if vec_map(y,x) > 0
        obj_cnt = obj_cnt + 1;
        obj_value = vec_map(y,x);
        obj_map(y,x) = obj_cnt;
        vec_map(y,x) = 0;
        all_objects_found = 1;
        while all_objects_found
          all_objects_found = 0;
          for y_map = 2 : 1 : vec_map_size_y - 1
            for x_map = 2 : 1 : vec_map_size_x - 1
              if( obj_map(y_map,x_map) == obj_cnt )
                search_area_obj = obj_map( y_map - floor( search_rad / 2 ) : y_map + floor( search_rad / 2 ),...
                                           x_map - floor( search_rad / 2 ) : x_map + floor( search_rad / 2 ) );
                search_area_value = vec_map( y_map - floor( search_rad / 2 ) : y_map + floor( search_rad / 2 ),...
                                             x_map - floor( search_rad / 2 ) : x_map + floor( search_rad / 2 ) );
                for search_y = 1 : 1 : search_rad
                  for search_x = 1 : 1 : search_rad
                    if search_area_obj(search_y,search_x) == 0 &&...
                      search_area_value(search_y,search_x) > 0 &&...
                      checkValueDistance(search_area_value(search_y,search_x), obj_value, 2*pi, d0)
                      all_objects_found = 1;
                      search_area_obj(search_y,search_x) = obj_cnt;
                      search_area_value(search_y,search_x) = 0;
                    end
                  end
                end
                obj_map( y_map - floor( search_rad / 2 ) : y_map + floor( search_rad / 2 ),...
                         x_map - floor( search_rad / 2 ) : x_map + floor( search_rad / 2 ) ) = search_area_obj;
                vec_map( y_map - floor( search_rad / 2 ) : y_map + floor( search_rad / 2 ),...
                         x_map - floor( search_rad / 2 ) : x_map + floor( search_rad / 2 ) ) = search_area_value;
              end
            end
          end
        end
      end
    end
  end
end