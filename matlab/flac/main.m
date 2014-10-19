%% reference implementation of the flac audio codec

% read file
% flac uses big endian format for its integer representation
fileID = fopen('test192.flac');
% reads bytewise
data = fread(fileID);

% first 4 bytes should read "fLaC"
if isequal(data(1:4), transpose(double('fLaC')))
    disp('Is a FLAC-file')
else
    fclose(fileID);
    error('No FLAC-file')
end
% read metadata blocks
metadataPointer = 5;
while 1
    metadataBlockHeader = data(metadataPointer);
    % bit 7 - 1 is block type
    blockType = bitand(metadataBlockHeader, hex2dec('fe'))
    % 24 bit = 3 byte matadatablock size
    blockSize = typecast(flip([0; uint8(data(1 + metadataPointer : 1 + 2 + metadataPointer))]), 'uint32');
    % point to new metadata or first file
    metadataPointer = metadataPointer + blockSize+ 4
  
    % is it the last metadata block?
    if bitget(metadataBlockHeader, 1) == 1
        break;
    end
    disp('Looping...');
    
end
disp('Finished reading all metadata');
% look for sync code
while 1
    val = bitand(typecast(flip([uint8(data(metadataPointer : 1 + metadataPointer))]), 'uint16'), hex2dec('3fff'));
    if bitxor(val, hex2dec('3FFE')) == 0
        break;
    end
    metadataPointer = metadataPointer + 2;
end
disp('Found Sync Block');
metadataPointer
% sync block 0x3FFE

fclose(fileID);