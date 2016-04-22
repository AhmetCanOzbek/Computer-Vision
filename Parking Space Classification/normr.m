function[y] = normr(x)
%NORMR Normalize rows of matrices.

if (ndims(x) ~= 2)
    error('Not Valid Input');
end

y = zeros(size(x,1),size(x,2));
for i = 1:size(x,1)
    %Calculate magnitude of the row and divide
    y(i,:) = x(i,:) / norm(x(i,:));   
end
