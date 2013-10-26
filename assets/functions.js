function isAbsolutePath(path) {
    return path.length > 0 && path.charAt(0) == '/';
}

function pathJoin(pathOne, pathTwo) {
    if(pathOne.length > 0) {
       if(pathOne.charAt(pathOne.length - 1) == '/') {
           return pathOne + pathTwo;
       } else {
           return pathOne + '/' + pathTwo
       }
    }
    return pathTwo
}

function fileNameOnly(path) {
    return path.replace(/.*\//, '');
}

function directoryOnly(path) {
    return path.substring(0, path.lastIndexOf('/'));
}

function replaceExtension(path, extension) {
    return path.substr(0, path.lastIndexOf(".")) + extension;
}

function getSizeKb(size) {
    var result;
    if(size < 1024) {
        result = qsTr("%1 bytes").arg(size);
    } else { 
        size /= 1024;
        if(size < 1024) {
            result = qsTr("%1 kB").arg(size.toFixed(2));
        } else {
            size /= 1024;
            if(size < 1024) {
                result = qsTr("%1 MB").arg(size.toFixed(2));
            } else {
                size /= 1024;
                result = qsTr("%1 GB").arg(size.toFixed(2));
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
        return qsTr("N/A");
    if(day_diff < 0)
        return qsTr("just now");
    return day_diff == 0 && (
           diff < 60 && qsTr("just now") ||
           diff < 120 && qsTr("1 minute ago") ||
           diff < 3600 && qsTr("%1 minutes ago").arg(Math.floor( diff / 60 )) ||
           diff < 7200 && qsTr("1 hour ago") ||
           diff < 86400 && qsTr("%1 hours ago").arg(Math.floor( diff / 3600 ))) ||
           
           day_diff == 1 && qsTr("yesterday") ||
           day_diff < 7 && qsTr("%1 days ago").arg(day_diff) ||
           day_diff < 31 && qsTr("%1 weeks ago").arg(Math.ceil(day_diff / 7)) ||
           day_diff < 365 && qsTr("%1 months ago").arg(Math.ceil(day_diff / 12)) ||
           qsTr("long ago");
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