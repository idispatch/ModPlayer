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

function formatTimeStamp(unix_timestamp) {
    var date = new Date(unix_timestamp * 1000);
    var diff = (((new Date()).getTime() - date.getTime()) / 1000);
    var day_diff = Math.floor(diff / 86400);
    if(isNaN(day_diff))
        return "N/A";
    if(day_diff < 0)
        return "just now";
    return day_diff == 0 && (
           diff < 60 && "just now" ||
           diff < 120 && "1 minute ago" ||
           diff < 3600 && Math.floor( diff / 60 ) + " minutes ago" ||
           diff < 7200 && "1 hour ago" ||
           diff < 86400 && Math.floor( diff / 3600 ) + " hours ago") ||
           
           (day_diff == 1 && "yesterday") ||
           (day_diff < 7 && day_diff + " days ago") ||
           (day_diff < 31 && Math.ceil( day_diff / 7 ) + " weeks ago") ||
           (day_diff < 365 && Math.ceil( day_diff / 12 ) + " months ago") ||
           ("long ago");
}

var entityMap = {
    "&": "&amp;",
    "<": "&lt;",
    ">": "&gt;"
};

function escapeHtml(string) {
    return String(string).replace(/[&<>]/g, function (s) {
        return entityMap[s];
    });
}