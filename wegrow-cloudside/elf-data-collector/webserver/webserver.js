// Create a div at the end of the document with the id
// "myfooter" and update it to demonstrate that javascript
// processing works in the simple server for a standard browser
// (i.e. show that the server correctly serves js files).

// Note that this is raw javascript.
// For a production site you would better off using libraries
// like angularjs or jquery to hide browser quirks.

window.onload = function() {
    // Find the 'myfooter' element.
    var elem = document.getElementById('myfooter');
    if (!!elem) {
        elem.innerHTML = '<p><i>Footer Information</i></p>';
    }
    else {
        alert('WARNING! Element "myfooter" not found.');
    }
};
