%% reference implementation of the flac audio codec

% read file
% flac uses big endian format for its integer representation
fileID = fopen('marvin.flac');
% reads bytewise
data = fread(fileID);
disp('============= Decoding =============')
% first 4 bytes should read "fLaC"
if isequal(data(1:4), transpose(double('fLaC')))
    disp('Is a FLAC-file')
else
    fclose(fileID);
    error('No FLAC-file')
end
% read metadata blocks
dataPointer = 5;
while 1
    metadataBlockHeader = data(dataPointer);
    % bit 7 - 1 is block type
    blockType = bitand(metadataBlockHeader, hex2dec('fe'));
    dataPointer = dataPointer + 1;
    
    % 24 bit = 3 byte matadatablock size
    blockSize = toInteger(data(dataPointer : 2 + dataPointer));
    dataPointer = dataPointer + 3;
    % this is the mantadory STREAMINFO block
    if blockType == 0 
        minBlockSize = toInteger(data(dataPointer : dataPointer + 1));
        maxBlockSize = toInteger(data(dataPointer + 2 : dataPointer + 3));
        minFrameSize = toInteger(data(dataPointer + 4 : dataPointer + 6));
        maxFrameSize = toInteger(data(dataPointer + 7 : dataPointer + 9));
        % probably not correct
        scrambledData = toInteger(data(dataPointer + 10 : dataPointer + 12));
        sampleRate =  bitand(scrambledData,  hex2dec('fffff'));
        nrOfChannels = bitshift(bitand(scrambledData,  hex2dec('700000')), -20);
    end
    
    % point to new metadata or first file
    dataPointer = dataPointer + blockSize;
    
    % was this the last metadata block?
    if bitget(metadataBlockHeader, 1) == 1
        break;
    end
    
end
disp('Finished reading all metadata');

% look for sync code
while 1
    val = bitand(toInteger(data(dataPointer : 1 + dataPointer)), hex2dec('3fff'));
    dataPointer = dataPointer + 1;
    
    if bitxor(val, hex2dec('3FFE')) == 0
        % we are below a sync block
        % TODO: check that we are really in a sync block through CRC
        % computing
        dataPointer = dataPointer + 1;
        
        blockSize = bitand(data(dataPointer), hex2dec('f'))
        sampleRate = bitshift(bitand(data(dataPointer), hex2dec('f0')), -4)
        dataPointer = dataPointer + 1;
        channelAssignment = bitand(data(dataPointer), hex2dec('f'))
        sampleSizeInBits = bitshift(bitand(data(dataPointer), hex2dec('70')), -4)
        break;
    end
    
end
disp('Found Sync Block');

% sync block 0x3FFE

fclose(fileID);