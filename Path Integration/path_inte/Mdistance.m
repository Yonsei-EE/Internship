function [d i k]= Mdistance(a,b,r,X,Y,onum,k)
      % onum은 장애물의 개수
      % (a,b)는 장애물의 위치
      % (X,Y)는 센서의 위치
      % d는 거리, i는 장애물 번호.
      
     
     for i = 1:1:onum    
         tmp(i) = ((X-a(i))^2+(Y-b(i))^2)^(0.5);
         if(r(i)>tmp(i))
             k = 1;
         end
     end
     [d i] = min(tmp);
end