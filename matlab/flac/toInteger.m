function [ output_args ] = toInteger( inputArray )
% Converts a transposed big endian array [1;2;3;..] of uint8 to an integer.
%   Reads an n-sized uint8 array and outputs the corresponding integer
%   value. For example:
%   [0x1, 0x2] = 0x102
%   Currently works for uint8, uint16, uint32 (24 bit gets padding
%   accordingly), uint64 (fewer bits as well padded).

inputArray = uint8(inputArray);

[m, n] = size(inputArray);
switch m
    case 1
       %trivial
       conversionTarget = 'uint8';
       convert = inputArray;
    case 2
        %trivial
        conversionTarget = 'uint16';
        convert = inputArray;
    case 3
        % padd according
        conversionTarget = 'uint32';
        convert = [0; inputArray];
    case 4
        % padd according        
        conversionTarget = 'uint32';
        convert = inputArray;
    case 5
        % padd according
        conversionTarget = 'uint64';
        convert = [0; 0; 0; inputArray];        
    case 6
        % padd according
        conversionTarget = 'uint64';
        convert = [0; 0; inputArray];   
    case 7
        % padd according
        conversionTarget = 'uint64';
        convert = [0; inputArray];   
    case 8
        % padd according        
        conversionTarget = 'uint64';
        convert = [inputArray];  
end
output_args = typecast(flip(convert), conversionTarget);
end

