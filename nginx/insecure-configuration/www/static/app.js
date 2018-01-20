window.onload = function() {
    var m = document.getElementById('m');
    m.innerHTML = location.hash.substr(1);
}