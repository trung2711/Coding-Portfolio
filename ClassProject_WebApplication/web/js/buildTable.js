// ********************** buildTable ***********************************
// list: an array of objects that are to be built into an HTML table.
// dbErrorMsg: a string that has a Db Error message (or empty string if all OK). 
// target: reference to DOM object where HTML table is to be placed (by buildTable). 

function buildTable(list, dbErrorMsg, target) {

    // prettyColumnHeading (private function). Pass in colHdg which was probably gotten from 
    // the property name in the JSON string (which initially came from java property name on server side).
    // Capitalize the first letter, then insert space before every subsequent capital letter. 
    // For example:  "userEmail" --> "User Email"
    function prettyColumnHeading(colHdg) {
        if (colHdg.length === 0) {
            return "";
        }
        
        // capitalize first letter
        newHdg = colHdg.charAt(0).toUpperCase();
        
        // iterate through all characters, inserting space before any capital letters.
        for (var i = 1; i < colHdg.length; i++) {
            if (colHdg.charAt(i) < "a") {
                newHdg += " ";
            }
            newHdg += colHdg.charAt(i);
        }
        return newHdg;
    } // end function prettyColumnHeading


    // Here starts the logic of function buildTable.
    if (dbErrorMsg === null) {
        console.log("Error: dbErrorMsg is null");
        target.innerHTML = "Error: dbErrorMsg is null";
        return;
    }

    if (dbErrorMsg.length > 0) {
        console.log("Database Error is: " + dbErrorMsg);
        target.innerHTML = "Database Error is: " + dbErrorMsg;
        return;
    }

    if (list === null) {
        console.log("list is null");
        target.innerHTML = "list is null";
        return;
    }

    if (list.length === 0) {
        console.log("list has 0 elements");
        target.innerHTML = "list has 0 elements";
        return;
    }

    // Create a new HTML table (DOM object) and append 
    // that into the page. 
    var newTable = document.createElement("table");
    target.appendChild(newTable);

    // Create a header for table and put a row in the header
    var tableHead = document.createElement("thead");
    newTable.appendChild(tableHead);
    var tableHeadRow = document.createElement("tr");
    tableHead.appendChild(tableHeadRow);

    // put first object (from array of objects) into a 
    // plain object (to avoid needing multi-dimensional array).
    // Extract property names from the object and use them 
    // as column headings.
    var data = list[0];
    for (prop in data) {
        var tableHeadDetail = document.createElement("th");
        tableHeadRow.appendChild(tableHeadDetail);
        tableHeadDetail.innerHTML = prettyColumnHeading(prop);
    }
    
    // Add one row (to HTML table) per element in the array.
    // Each array element has a list of properties that will become 
    // td elements (Table Data, a cell) in the HTML table. 
    var tableBody = document.createElement("tbody");
    newTable.appendChild(tableBody);
    for (var i in list) {
        var tableRow = document.createElement("tr");
        tableBody.appendChild(tableRow);
        var data = list[i];

        // "prop" iterates over the properties in object "data"
        for (var prop in data) {
            var tableData = document.createElement("td");
            tableRow.appendChild(tableData);
            tableData.innerHTML = data[prop];
            align(tableData); // I/O parameter
        }
    }

    function align(tableData) { //right justify HTML table data (cell) if it contains a number

        var cellContent = tableData.innerHTML;
        if (!isNaN(cellContent) || // if numeric 
                ((cellContent.length > 0) && (cellContent.charAt(0) === "$"))) { // or dollar amt
            tableData.style.textAlign = "right";
            console.log("right alligning " + cellContent);
        }
        
        if (cellContent.includes("img") && (cellContent.charAt(cellContent.length-1) === ">")) {
            tableData.style.textAlign = "center";
        }
    } // end function align

} // end function buildTable