function [d i k]= Mdistance(a,b,r,X,Y,onum,k)
      % onum�� ��ֹ��� ����
      % (a,b)�� ��ֹ��� ��ġ
      % (X,Y)�� ������ ��ġ
      % d�� �Ÿ�, i�� ��ֹ� ��ȣ.
      
     
     for i = 1:1:onum    
         tmp(i) = ((X-a(i))^2+(Y-b(i))^2)^(0.5);
         if(r(i)>tmp(i))
             k = 1;
         end
     end
     [d i] = min(tmp);
end