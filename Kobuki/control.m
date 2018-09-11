function control(s,speed,r)
    header = [170 85 6 1 4];
    command = checksum(header,speed,r);
    fwrite(s,command);
end

