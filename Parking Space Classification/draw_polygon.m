function[] = draw_polygon(cx,cy,varargin)

    if(nargin > 2)
        color = varargin{1};       
    else      
        %Default color blue
        color = 'b';
    end
    
    
    %Draw the lines
    hold on;
    plot([cx(1) cx(2)],[cy(1) cy(2)],color); hold on; %1-2
    plot([cx(1) cx(3)],[cy(1) cy(3)],color); hold on; %1-3
    plot([cx(1) cx(4)],[cy(1) cy(4)],color); hold on; %1-4
    plot([cx(2) cx(3)],[cy(2) cy(3)],color); hold on; %2-3
    plot([cx(2) cx(4)],[cy(2) cy(4)],color); hold on; %2-4
    plot([cx(3) cx(4)],[cy(3) cy(4)],color); hold on; %3-4

end