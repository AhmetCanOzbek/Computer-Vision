function[word_histogram] = get_im_words(im,cluster_means)
    %Gets the word histogram of an image
    n_words = size(cluster_means,1);
    
    %Extract SIFT features
    I = single(rgb2gray(im)); 
    [f,descriptors] = vl_sift(I);
    descriptors = descriptors';
    
    word_histogram = zeros(1,n_words);
    n_descriptors = max(size(descriptors,1));
    
    
    for i=1:n_descriptors
        closest_index = get_closest_index(descriptors(i,:),cluster_means);
        word_histogram(1,closest_index) = word_histogram(1,closest_index) + 1;       
    end
    
    %Maybe normalize here
    
end

function[index] = get_closest_index(vector,cluster_means)
    
    n_clusters = size(cluster_means,1);
    
    distances = [];
    for i=1:n_clusters
        %Calculate norm
        vector = double(vector);        
        distances(i) = norm( (vector) - (cluster_means(i,:)) );       
    end
    
    [min_value,index] = min(distances);

end

