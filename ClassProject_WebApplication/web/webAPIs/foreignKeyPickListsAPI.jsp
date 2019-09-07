<%@page contentType="application/json; charset=UTF-8" pageEncoding="UTF-8"%> 

<%@page language="java" import="dbUtils.*" %>
<%@page language="java" import="model.misc.*" %>  
<%@page language="java" import="com.google.gson.*" %>

<%

    DbConn dbc = new DbConn();
    ForeignKeyLists lists = new ForeignKeyLists(dbc);

   // PREVENT DB connection leaks:
    dbc.close(); // EVERY code path that opens a db connection, must also close it.

    Gson gson = new Gson();
    out.print(gson.toJson(lists).trim());
%>