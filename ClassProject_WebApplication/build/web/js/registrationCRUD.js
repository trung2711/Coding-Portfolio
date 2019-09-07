var registrationCRUD = {}; // globally available object

(function () {

registrationCRUD.startInsert = function () {

    ajax('htmlPartials/insertUpdateRegistration.html', setInsertUI, 'content');

    function setInsertUI(httpRequest) {

        console.log("Ajax call was successful.");
        document.getElementById("content").innerHTML = httpRequest.responseText;
        
        ajax("webAPIs/foreignKeyPickListsAPI.jsp", setPickList, "Error");
        function setPickList(httpRequest) {
            console.log("setPickList was called");
            var jsonObj = JSON.parse(httpRequest.responseText); // convert from JSON to JS Object.
            console.log(jsonObj);
            if (jsonObj.dbError.length > 0) {
                document.getElementById("tournamentError").innerHTML = jsonObj.dbError;
                return;
            }
            makePickList(jsonObj.userPickList, "webUserId", "userEmail", "userPickList");
            makePickList(jsonObj.tournamentPickList, "tournamentId", "tournamentName", "tournamentPickList");
        }
    }
};    
    
registrationCRUD.insertSave = function () {
    console.log ("registrationCRUD.insertSave was called");

    var userList = document.getElementById("userPickList");
    var tournamentList = document.getElementById("tournamentPickList");

    var registrationInputObj = {
        "registrationId": "",
        "registrationDate": document.getElementById("registrationDate").value,
        "webUserId": userList.options[userList.selectedIndex].value,
        "userEmail": "",
        "additionalNotes": document.getElementById("additionalNotes").value,
        "percentAsDonation": document.getElementById("donation").value,
        "tournamentId": tournamentList.options[tournamentList.selectedIndex].value,
        "tournamentName": "",
        "tournamentCapacity": "",
        "tournamentPrize": "",
        "tournamentDate": "",
        "tournamentTime": "",
        "tournamentRegistrationDeadline": "",
        
        "errorMsg": ""
    };
    console.log(registrationInputObj);

    var myData = escape(JSON.stringify(registrationInputObj));
    var url = "webAPIs/insertAssocAPI.jsp?jsonData=" + myData;
    ajax(url, processInsert, "recordError");

    function processInsert(httpRequest) {
        console.log("processInsert was called here is httpRequest.");
        console.log(httpRequest);

        // the server prints out a JSON string of an object that holds field level error 
        // messages. The error message object (conveniently) has its fiels named exactly 
        // the same as the input data was named. 
        var jsonObj = JSON.parse(httpRequest.responseText); // convert from JSON to JS Object.
        console.log("here is JSON object (holds error messages.");
        console.log(jsonObj);

        document.getElementById("userError").innerHTML = jsonObj.userEmail;
        document.getElementById("tournamentError").innerHTML = jsonObj.tournamentName;
        document.getElementById("dateError").innerHTML = jsonObj.registrationDate;
        document.getElementById("donationError").innerHTML = jsonObj.percentAsDonation;
        document.getElementById("additionalNotesError").innerHTML = jsonObj.additionalNotes;

        if (jsonObj.errorMsg.length === 0) { // success
            jsonObj.errorMsg = "Record successfully inserted !!!";
        }
        document.getElementById("recordError").innerHTML = jsonObj.errorMsg;
    }
};

registrationCRUD.list = function (targetId) {

    var dataList = document.createElement("div");
    dataList.id = "dataList"; // set the id so it matches CSS styling rules in listStyle.css
    dataList.innerHTML = "<h2>Registrations <img src='pics/insert.png' style='height:25px;width:25px;' onclick='registrationCRUD.startInsert();'/></h2></h2>";
    dataList.innerHTML += "<h3 id='listMsg'></h3>";

    document.getElementById(targetId).innerHTML = "";
    document.getElementById(targetId).appendChild(dataList);

    ajax('webAPIs/listAssocAPI.jsp', setListUI, 'listMsg');

    function setListUI(httpRequest) {

        console.log("starting registrationCRUD.list (setListUI) with this httpRequest object (next line)");
        console.log(httpRequest);

        var obj = JSON.parse(httpRequest.responseText);

        if (obj === null) {
            document.getElementById("listMsg").innerHTML = "registrationCRUD.list Error: JSON string evaluated to null.";
            return;
        }

        for (var i = 0; i < obj.registration.length; i++) {

            // how you would add a new property if you wanted to
            if(obj.registration[i].tournamentRegistrationDeadline === ""){
                obj.registration[i].dateInformation = "Rolling\n";
            } else {
            obj.registration[i].dateInformation = "Registration deadline: " + obj.registration[i].tournamentRegistrationDeadline + '<br>' + "Start date: " +
                    obj.registration[i].tournamentDate + '<br>' + "Start time: " + obj.registration[i].tournamentTime;
            }
            
            obj.registration[i].delete = "<img src='pics/delete.png'  onclick='registrationCRUD.delete(" + obj.registration[i].registrationId + ",this)'  />";
            
            // how to delete properties
            delete obj.registration[i].tournamentRegistrationDeadline;
            delete obj.registration[i].tournamentDate;
            delete obj.registration[i].tournamentTime;
            delete obj.registration[i].tournamentId;
            delete obj.registration[i].webUserId;
        }
        
        // buildTable Parameters: 
        // First:  array of objects that are to be built into an HTML table.
        // Second: string that is database error (if any) or empty string if all OK.
        // Third:  reference to DOM object where built table is to be stored. 
        buildTable(obj.registration, obj.dbError, dataList);
    }
};

registrationCRUD.delete = function (registrationId, icon) {
        if (confirm("Do you really want to delete registration " + registrationId + "? ")) {
            console.log("icon that was passed into JS function is printed on next line");
            console.log(icon);
            
            ajax("webAPIs/deleteAssocAPI.jsp?deleteId="+registrationId, modify, null);
            // HERE YOU HAVE TO CALL THE DELETE API and the success function should run the 
            // following (delete the row that was clicked from the User Interface).
            function modify(httpRequest){
                // icon's parent is cell whose parent is row
                var jsonObj = JSON.parse(httpRequest.responseText);
                if(jsonObj.errorMsg.length>0){
                    alert(jsonObj.errorMsg.toString());
                } else {
                    alert("Success!");
                    var dataRow = icon.parentNode.parentNode;
                    var rowIndex = dataRow.rowIndex - 1; // adjust for oolumn header row?
                    var dataTable = dataRow.parentNode;
                    dataTable.deleteRow(rowIndex);
                }   
            }
        }

}; //delete

}());