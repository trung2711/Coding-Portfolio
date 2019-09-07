<%@page contentType="application/json; charset=UTF-8" pageEncoding="UTF-8"%> 

<%@page language="java" import="dbUtils.*" %>
<%@page language="java" import="model.webUser.*" %> 
<%@page language="java" import="view.WebUserView" %> 
<%@page language="java" import="com.google.gson.*" %>

<%

    // default constructor creates nice empty StringDataList with all fields "" (empty string, nothing null).
    StringData found = (StringData) session.getAttribute("webUser");
    
    if (found  == null) {
        found = new StringData();
        found.errorMsg = "Cannot provide profile - you are not logged on.";
        
    }
    
    // This object (from the GSON library) can to convert between JSON <-> POJO (plain old java object)  
    Gson gson = new Gson();
    out.print(gson.toJson(found).trim());
%>
