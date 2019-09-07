var tournamentCRUD = {}; // globally available object

//Code for inserting tournament is based on sample code for inserting user
(function () {

tournamentCRUD.startInsert = function () {

    ajax('htmlPartials/insertUpdateTournament.html', setInsertUI, 'content');

    function setInsertUI(httpRequest) {

        console.log("Ajax call was successful.");
        document.getElementById("content").innerHTML = httpRequest.responseText;
        
        document.getElementById("updateSaveTournamentButton").style.display = "none";
        document.getElementById("tournamentIdRow").style.display = "none";
    }
};    
    
tournamentCRUD.insertSave = function () {

    console.log ("tournamentCRUD.insertSave was called");

    var tournamentInputObj = {
        "tournamentId": "",
        "tournamentName": document.getElementById("tournamentName").value,
        "tournamentCapacity": document.getElementById("tournamentCapacity").value,
        "tournamentPrize": document.getElementById("tournamentPrize").value,
        "tournamentDescription": document.getElementById("tournamentDescription").value,
        "tournamentDate": document.getElementById("tournamentDate").value,
        "tournamentTime": document.getElementById("tournamentTime").value,
        "tournamentRegistrationDeadline": document.getElementById("tournamentRegistrationDeadline").value,
        "imageUrl": document.getElementById("imageUrl").value,
        "discordUrl": document.getElementById("discordUrl").value,

        "errorMsg": ""
    };
    console.log(tournamentInputObj);

    var myData = escape(JSON.stringify(tournamentInputObj));
    var url = "webAPIs/insertTournamentAPI.jsp?jsonData=" + myData;
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

        document.getElementById("tournamentNameError").innerHTML = jsonObj.tournamentName;
        document.getElementById("tournamentCapacityError").innerHTML = jsonObj.tournamentCapacity;
        document.getElementById("tournamentPrizeError").innerHTML = jsonObj.tournamentPrize;
        document.getElementById("tournamentDescriptionError").innerHTML = jsonObj.tournamentDescription;
        document.getElementById("tournamentDateError").innerHTML = jsonObj.tournamentDate;
        document.getElementById("tournamentTimeError").innerHTML = jsonObj.tournamentTime;
        document.getElementById("tournamentRegistrationDeadlineError").innerHTML = jsonObj.tournamentRegistrationDeadline;
        document.getElementById("imageUrlError").innerHTML = jsonObj.imageUrl;
        document.getElementById("discordUrlError").innerHTML = jsonObj.discordUrl;

        if (jsonObj.errorMsg.length === 0) { // success
            jsonObj.errorMsg = "Record successfully inserted !!!";
        }
        document.getElementById("recordError").innerHTML = jsonObj.errorMsg;
    }
};

tournamentCRUD.list = function (targetId) {

    var dataList = document.createElement("div");
    dataList.id = "dataList"; // set the id so it matches CSS styling rules in listStyle.css
    dataList.innerHTML = "<h2>Tournaments <img src='pics/insert.png' style='height:25px;width:25px;' onclick='tournamentCRUD.startInsert();'/></h2>";
    dataList.innerHTML += "<h3 id='listMsg'></h3>";

    document.getElementById(targetId).innerHTML = "";
    document.getElementById(targetId).appendChild(dataList);

    ajax('webAPIs/listOtherAPI.jsp', setListUI, 'listMsg');

    function setListUI(httpRequest) {

        console.log("starting tournamentCRUD.list (setListUI) with this httpRequest object (next line)");
        console.log(httpRequest);

        var obj = JSON.parse(httpRequest.responseText);
        
        if (obj === null) {
            document.getElementById("listMsg").innerHTML = "tournamentCRUD.list Error: JSON string evaluated to null.";
            return;
        }

        for (var i = 0; i < obj.tournament.length; i++) {
            
            if(obj.tournament[i].imageUrl !== ""){
                obj.tournament[i].tournamentName = "<img src=\"" + obj.tournament[i].imageUrl + "\" style=\"width:140px;height:60px;\">" + "</br>" + obj.tournament[i].tournamentName;
            }
            if(obj.tournament[i].discordUrl !== ""){
                obj.tournament[i].discordUrl = "<a href=\"" + obj.tournament[i].discordUrl + "\">Discord</a>";
            }
            // how you would add a new property if you wanted to
            if(obj.tournament[i].tournamentRegistrationDeadline === ""){
                obj.tournament[i].dateInformation = "Rolling\n";
            } else {
                obj.tournament[i].dateInformation = "Registration deadline: " + "</br>" + obj.tournament[i].tournamentRegistrationDeadline + "</br>" + "\nStart date: " + "</br>" +
                obj.tournament[i].tournamentDate + "</br>" + "\nStart time: " + "</br>" + obj.tournament[i].tournamentTime;
            }
            
            obj.tournament[i].delete = "<img src='pics/delete.png'  onclick='tournamentCRUD.delete(" + obj.tournament[i].tournamentId + ",this)'  />";
            obj.tournament[i].update = "<img onclick='tournamentCRUD.startUpdate(" + obj.tournament[i].tournamentId + ")' src='pics/update.png' />";
            // how to delete properties
            delete obj.tournament[i].imageUrl;
            delete obj.tournament[i].tournamentRegistrationDeadline;
            delete obj.tournament[i].tournamentDate;
            delete obj.tournament[i].tournamentTime;
        }
        
        // buildTable Parameters: 
        // First:  array of objects that are to be built into an HTML table.
        // Second: string that is database error (if any) or empty string if all OK.
        // Third:  reference to DOM object where built table is to be stored. 
        buildTable(obj.tournament, obj.dbError, dataList);
    }
};

tournamentCRUD.delete = function (tournamentId, icon) {
        if (confirm("Do you really want to delete tournament " + tournamentId + "? ")) {
            console.log("icon that was passed into JS function is printed on next line");
            console.log(icon);
            
            ajax("webAPIs/deleteOtherAPI.jsp?deleteId="+tournamentId, modify, null);
            // HERE YOU HAVE TO CALL THE DELETE API and the success function should run the 
            // following (delete the row that was clicked from the User Interface).
            function modify(httpRequest){
                // icon's parent is cell whose parent is row
                var jsonObj = JSON.parse(httpRequest.responseText);
                if(jsonObj.errorMsg.length>0){
                   if(jsonObj.errorMsg.includes("CONSTRAIN")){
                        alert("Cannot delete: one or more users are registered to this tournament.")
                    } else {
                    alert(jsonObj.errorMsg.toString());
                    }
                } else {
                    alert("Success!");
                    var dataRow = icon.parentNode.parentNode;
                    var rowIndex = dataRow.rowIndex - 1; // adjust for oolumn header row?
                    var dataTable = dataRow.parentNode;
                    dataTable.deleteRow(rowIndex);
                }   
            }
        }

};
    
    function getTournamentDataFromUI() {
        
        var dollar = stripDollar(document.getElementById("tournamentPrize").value);

        var tournamentInputObj = {
            "tournamentId": document.getElementById("tournamentId").value,
            "tournamentName": document.getElementById("tournamentName").value,
            "tournamentCapacity": document.getElementById("tournamentCapacity").value,
            "tournamentPrize": dollar,
            "tournamentDescription": document.getElementById("tournamentDescription").value,
            "tournamentDate": document.getElementById("tournamentDate").value,
            "tournamentTime": document.getElementById("tournamentTime").value,
            "tournamentRegistrationDeadline": document.getElementById("tournamentRegistrationDeadline").value,
            "imageUrl": document.getElementById("imageUrl").value,
            "discordUrl": document.getElementById("discordUrl").value
        };

        console.log(tournamentInputObj);

        // build the url for the ajax call. Remember to escape the user input object or else 
        // you'll get a security error from the server. JSON.stringify converts the javaScript
        // object into JSON format (the reverse operation of what gson does on the server side).
        return escape(JSON.stringify(tournamentInputObj));
    }

    function writeErrorObjToUI(jsonObj) {
        console.log("here is JSON object (holds error messages.");
        console.log(jsonObj);

        document.getElementById("tournamentNameError").innerHTML = jsonObj.tournamentName;
        document.getElementById("tournamentCapacityError").innerHTML = jsonObj.tournamentCapacity;
        document.getElementById("tournamentPrizeError").innerHTML = jsonObj.tournamentPrize;
        document.getElementById("tournamentDescriptionError").innerHTML = jsonObj.tournamentDescription;
        document.getElementById("tournamentDateError").innerHTML = jsonObj.tournamentDate;
        document.getElementById("tournamentTimeError").innerHTML = jsonObj.tournamentTime;
        document.getElementById("tournamentRegistrationDeadlineError").innerHTML = jsonObj.tournamentRegistrationDeadline;
        document.getElementById("imageUrlError").innerHTML = jsonObj.imageUrl;
        document.getElementById("discordUrlError").innerHTML = jsonObj.discordUrl;

        document.getElementById("recordError").innerHTML = jsonObj.errorMsg;
    }

// user has clicked on an update icon from the web_user list UI. 
// inject the insert/update web_user UI into the content area and pre-fill 
// that with the web_user data exracted from the database. 
    tournamentCRUD.startUpdate = function (tournamentId) {

        console.log("startUpdate");

        // make ajax call to get the insert/update user UI
        ajax('htmlPartials/insertUpdateTournament.html', setUpdateUI, "content");

        // place the insert/update user UI into the content area
        function setUpdateUI(httpRequest) {
            console.log("Ajax call was successful.");
            document.getElementById("content").innerHTML = httpRequest.responseText;

            document.getElementById("insertSaveTournamentButton").style.display = "none";
            //document.getElementById("updateSaveUserButton").style.display = "inline";

            // Call the Get User by id API and (if success), fill the UI with the User data
            ajax("webAPIs/getTournamentByIdAPI.jsp?id=" + tournamentId, displayTournament, "recordError");

            function displayTournament(httpRequest) {
                var obj = JSON.parse(httpRequest.responseText);
                if (obj.errorMsg.length > 0) {
                    document.getElementById("recordError").innerHTML = "Database error: " +
                            obj.errorMsg;
                } else if (obj.tournamentId.length < 1) {
                    document.getElementById("recordError").innerHTML = "There is no tournament with id '" +
                            tournamentId + "' in the database";
                } else {
                    //change
                    document.getElementById("tournamentId").value = obj.tournamentId;
                    document.getElementById("tournamentName").value = obj.tournamentName;
                    document.getElementById("tournamentCapacity").value = obj.tournamentCapacity;
                    document.getElementById("tournamentPrize").value = obj.tournamentPrize;
                    document.getElementById("tournamentDescription").value = obj.tournamentDescription;
                    document.getElementById("tournamentDate").value = obj.tournamentDate;
                    document.getElementById("tournamentTime").value = obj.tournamentTime;
                    document.getElementById("tournamentRegistrationDeadline").value = obj.tournamentRegistrationDeadline;
                    document.getElementById("imageUrl").value = obj.imageUrl;
                    document.getElementById("discordUrl").value = obj.discordUrl;      
                }
            }
        } // setUpdateUI
    };

    tournamentCRUD.updateSave = function () {

        console.log("tournamentCRUD.updateSave was called");
        var myData = getTournamentDataFromUI();
        var url = "webAPIs/updateOtherAPI.jsp?jsonData=" + myData;
        ajax(url, processUpdate, "recordError");

        function processUpdate(httpRequest) {
            console.log("processUpdate was called here is httpRequest.");
            console.log(httpRequest);

            // the server prints out a JSON string of an object that holds field level error 
            // messages. The error message object (conveniently) has its fields named exactly 
            // the same as the input data was named. 
            var jsonObj = JSON.parse(httpRequest.responseText); // convert from JSON to JS Object.
            console.log("here is JSON object (holds error messages.");
            console.log(jsonObj);

            if (jsonObj.errorMsg.length === 0) { // success
                jsonObj.errorMsg = "Record successfully updated !!!";
            }

            writeErrorObjToUI(jsonObj);

        }
    };

    // remove commas and $ from user entered dollar amount.
    // private helper function, availble to any functions in the IIFE
    function stripDollar(dollar) {
        dollar = dollar.replace("$", ""); // replace $ with empty string
        dollar = dollar.replace(",", ""); // replace comma with empty string
        return dollar;
    }
}());

