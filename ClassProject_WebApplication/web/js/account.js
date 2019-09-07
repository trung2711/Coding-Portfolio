/*
 * Code is based on Dr. Kyvernitis's implementation of account function
 */
var account={};
account.logon = function(emailId, pwId, msgId){
    var emailUserInput = escape(document.getElementById(emailId).value);
    var pwUserInput = escape(document.getElementById(pwId).value);
    
    ajax("webAPIs/logonAPI.jsp?email=" + emailUserInput + "&password=" + pwUserInput, processLogon, msgId);
    
    function processLogon(httpRequest){
        console.log("starting processLogon");
        console.log(httpRequest);
        
        var obj = JSON.parse(httpRequest.responseText);
        
        if (obj.errorMsg !== "") {
            document.getElementById(msgId).innerHTML = "<h4>"+obj.errorMsg+"</h4>";
        } else {
            document.getElementById(msgId).innerHTML = "<h4>Welcome web user " + obj.userEmail + "! <br>Your info:<br>  Birthday: " +obj.birthday+ "<br>    Membership Fee: "+ obj.membershipFee + "<br>    User Role ID: " + obj.userRoleId + "<br>    User Role Type: " + obj.userRoleType +"</h4>";            
        }
    }
};

account.logoff = function(msgId){
    ajax("webAPIs/logoffAPI.jsp", loggedOffMsg, msgId);
    
    function loggedOffMsg (httpRequest) {
        console.log("starting loggedOffMsg");
        console.log(httpRequest);
        
        document.getElementById(msgId).innerHTML = "<h4>"+httpRequest.responseText+"</h4>";
    }
};

account.getProfile = function(msgId){
    ajax("webAPIs/getProfileAPI.jsp", profileMsg, msgId);
    
    function profileMsg (httpRequest) {
        console.log("starting profileMsg");
        console.log(httpRequest);
        
        var obj = JSON.parse(httpRequest.responseText);
        
        if (obj.errorMsg !== "") {
            document.getElementById(msgId).innerHTML = "<h4>" + obj.errorMsg +"</h4>";
        } else {
            document.getElementById(msgId).innerHTML = "<h3>Your Profile:</h3>";
            document.getElementById(msgId).innerHTML += "<h4>Email: </h4>"+obj.userEmail;
            document.getElementById(msgId).innerHTML += "<h4>Birthday: </h4>"+obj.birthday;
            document.getElementById(msgId).innerHTML += "<h4>Membership Fee: </h4>"+ obj.membershipFee;
            document.getElementById(msgId).innerHTML += "<h4>User Role ID: </h4>"+ obj.userRoleId;
            document.getElementById(msgId).innerHTML += "<h4>User Role Type: </h4>"+ obj.userRoleType;
        }
    }
};
