<%@page contentType="application/json; charset=UTF-8" pageEncoding="UTF-8"%> 

<%@page language="java" import="dbUtils.*" %>
<%@page language="java" import="model.webUser.*" %> 
<%@page language="java" import="view.WebUserView" %> 
<%@page language="java" import="com.google.gson.*" %>

<%

    // default constructor creates nice empty StringDataList with all fields "" (empty string, nothing null).
    StringData data = new StringData();

    String strEmail = request.getParameter("email");
    String strPass = request.getParameter("password"); 
    
    if (strEmail == null || strPass == null) {
        data.errorMsg = "Email and password fields are null";
    } else {
        DbConn dbc = new DbConn();
        data.errorMsg = dbc.getErr();
        
        if(data.errorMsg.length()==0){
            System.out.println("*** Ready to call logonAPI");
            if((data=DbMods.logonFind(strEmail, strPass, dbc))==null){
                data = new StringData();
                data.errorMsg = "Wrong email & password!";
            } else {
                session.setAttribute ("webUser", data);
            }
        }
        
        
        
        dbc.close(); // EVERY code path that opens a db connection, must also close it - no DB Conn leaks.
    }
    
    Gson gson = new Gson();
    out.print(gson.toJson(data).trim());
    // This object (from the GSON library) can to convert between JSON <-> POJO (plain old java object) 
%>