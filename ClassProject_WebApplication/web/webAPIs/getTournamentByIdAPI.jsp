<%@page contentType="application/json; charset=UTF-8" pageEncoding="UTF-8"%> 

<%@page language="java" import="dbUtils.*" %>
<%@page language="java" import="model.tournament.*" %> 
<%@page language="java" import="view.*" %> 
<%@page language="java" import="com.google.gson.*" %>

<%
    StringData tournament = new StringData();
    
    String searchId = request.getParameter("id");
    if (searchId == null) {
        tournament.errorMsg = "Cannot search for tournament - 'id' most be supplied as URL parameter";
    } else {
        DbConn dbc = new DbConn();
        tournament.errorMsg = dbc.getErr(); // returns "" if connection is good, else db error msg.

        if (tournament.errorMsg.length() == 0) { // if got good DB connection,

            System.out.println("*** Ready to call getTournamentById");
            tournament = Search.getTournamentById(dbc, searchId);
             
        }

        dbc.close(); // EVERY code path that opens a db connection, must also close it - no DB Conn leaks.
    }
    // This object (from the GSON library) can to convert between JSON <-> POJO (plain old java object) 
    Gson gson = new Gson();
    out.print(gson.toJson(tournament).trim());
%>
