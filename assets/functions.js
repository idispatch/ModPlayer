function getSizeKb(size) {
    var result;
    if(size < 1024) {
        result = size + " bytes";
    } else { 
        size /= 1024;
        if(size < 1024) {
            result = size.toFixed(2) + " kB";
        } else {
            size /= 1024;
            if(size < 1024) {
                result = size.toFixed(2) + " MB";
            } else {
                size /= 1024;
                result = size.toFixed(2) + " GB";
            }
        }
    }
    return result; 
}