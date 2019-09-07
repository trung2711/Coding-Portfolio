
// ********************** ajax *************************************   
// Make an ajax call to the given url, then if the call was successful, 
// call the Success Callback fn, otherwise, set an error message into the 
// DOM element that has id 'errorId'.
function ajax(url, callBackSuccess, errorId) {

// The httpReq Object is now local to function "ajaxCall" (not global)
    var httpReq;
    if (window.XMLHttpRequest) {
        httpReq = new XMLHttpRequest(); //For Firefox, Safari, Opera
    } else if (window.ActiveXObject) {
        httpReq = new ActiveXObject("Microsoft.XMLHTTP"); //For IE 5+
    } else {
        alert('ajax not supported');
    }

    console.log("ready to get content " + url);
    httpReq.open("GET", url); // specify which page you want to get

    // Ajax calls are asyncrhonous (non-blocking). Specify the code that you 
    // want to run when the response (to the http request) is available. 
    httpReq.onreadystatechange = function () {

        // readyState == 4 means that the http request is complete
        if (httpReq.readyState === 4) {
            if (httpReq.status === 200) {
                callBackSuccess(httpReq);
            } else {
                // First use of property creates new (custom) property
                document.getElementById(errorId).innerHTML = "Error (" + httpReq.status + " " + httpReq.statusText +
                        ") while attempting to read '" + url + "'";
            }
        }
    }; // end of anonymous function

    httpReq.send(null); // initiate ajax call

} // end function ajax

// Make an ajax call to the given url. Then, once the call has been made 
// (and data is available), place the response text from the ajax call into the 
// inner html property of the DOM elemennt with the given id.
function ajaxFillId(url, id) {

    // The httpReq Object is now local to function "ajaxCall" (not global)
    var httpReq;
    if (window.XMLHttpRequest) {
        httpReq = new XMLHttpRequest();  //For Firefox, Safari, Opera
    } else if (window.ActiveXObject) {
        httpReq = new ActiveXObject("Microsoft.XMLHTTP");    //For IE 5+
    } else {
        alert('ajax not supported');
    }

    console.log("ready to get content " + url);
    httpReq.open("GET", url); // specify which page you want to get


    // Ajax calls are asyncrhonous (non-blocking). Specify the code that you 
    // want to run when the response (to the http request) is available. 
    httpReq.onreadystatechange = function () {

        // readyState == 4 means that the http request is complete
        if (httpReq.readyState === 4) {
            if (httpReq.status === 200) {
                document.getElementById(id).innerHTML = httpReq.responseText;
            } else {
                // First use of property creates new (custom) property
                document.getElementById(id).innerHTML = "Error (" +
                        httpReq.status + " " + httpReq.statusText +
                        ") while attempting to read '" + url + "'";
            }
        }
    };  // end of anonymous function

    httpReq.send(null); // initiate ajax call

} // end function ajaxFillId


// Get reference to DOM element with the given id. If that element is currently visible, 
// then make it hidden and set the right attribute way off to the right (out of sight). 
// If the element is currently hidden, set it to visible and set the right attribute 
// according to where the web designer wants the drop down to be show up left/right (in pixels)-
// the top/bottom is set by the web designer (in CSS) according to the height of the title area. 
// Note: you ave to use the "visibility" (CSS) rather than "display" if you want to use 
// transitions (provide the animation for opening and closing the Drop down).  
function toggle(id, right) {

    // get reference to the dropdown (parent) that is to be opened or closed
    var ele = document.getElementById(id);

    // when one dropdown is clicked, make sure to close any other ones
    // that the user may have left open.
    var dropdowns = document.getElementsByClassName("dropDown");
    console.log(dropdowns);
    for (var i = 0; i < dropdowns.length; i++) {
        if (dropdowns[i] !== ele) {
            dropdowns[i].style.right = "-500px";  // set it off screen to right so animates next opening
            dropdowns[i].style.visibility = "hidden";
        }
    }

    // Tip: JS doesnt understand the initial CSS values (the values 
    // set by style sheet instead of by JS), unless you use a function that is 
    // something like "getComputedStyle". But you can avoid this by having the 
    // if condition test for the way CSS does NOT have it initially set. 
    // (In other words, do not reverse the order of the if/else block.)                      
    if (ele.style.visibility === "visible") {
        ele.style.right = "-500px"; // set it off screen to right so animates next opening
        ele.style.visibility = "hidden";

    } else {
        ele.style.visibility = "visible";
        ele.style.right = right + "px";
    }
} // end function toggle
     
