function angle = findAngle( dy, dx )
  if dx == 0
    dx = realmin;
  end
  angle = atan( double( dy ) / double( dx ) );
  if dx < 0
    angle = angle + pi;
  elseif dy < 0
    angle = angle + 2*pi;
  end
end