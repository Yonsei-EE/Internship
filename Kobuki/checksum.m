function command = checksum(header,speed,r)
    check = 0;
    temp = [header speed r];
    for i=3:header(3)+1
        check = check + temp(i);
    end
    command = [header speed r check];
end