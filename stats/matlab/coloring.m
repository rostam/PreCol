function [num_color,colors] = coloring(A)
    %number of needed colors
    num_color = 1;
    colors = [];
    A(A~=0)=1;
%    C = sparse(zeros(size(A)));
    C = sparse(size(A,1),size(A,2));

    for i = 1:size(A,1)
        for j = 1:num_color
            if not (any(C(:,j)+A(:,i)>1))
                C(:,j) = C(:,j) + A(:,i);
                C(C>1) = 1;
                %update num_color if necessary
                num_color = max(num_color,j+1);
                colors = [colors j];
                break
            end
        end
    end
                
    num_color = max(colors);