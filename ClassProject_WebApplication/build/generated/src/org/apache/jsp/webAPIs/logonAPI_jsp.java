package org.apache.jsp.webAPIs;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.jsp.*;
import dbUtils.*;
import model.webUser.*;
import view.WebUserView;
import com.google.gson.*;

public final class logonAPI_jsp extends org.apache.jasper.runtime.HttpJspBase
    implements org.apache.jasper.runtime.JspSourceDependent {

  private static final JspFactory _jspxFactory = JspFactory.getDefaultFactory();

  private static java.util.List<String> _jspx_dependants;

  private org.glassfish.jsp.api.ResourceInjector _jspx_resourceInjector;

  public java.util.List<String> getDependants() {
    return _jspx_dependants;
  }

  public void _jspService(HttpServletRequest request, HttpServletResponse response)
        throws java.io.IOException, ServletException {

    PageContext pageContext = null;
    HttpSession session = null;
    ServletContext application = null;
    ServletConfig config = null;
    JspWriter out = null;
    Object page = this;
    JspWriter _jspx_out = null;
    PageContext _jspx_page_context = null;

    try {
      response.setContentType("application/json; charset=UTF-8");
      pageContext = _jspxFactory.getPageContext(this, request, response,
      			null, true, 8192, true);
      _jspx_page_context = pageContext;
      application = pageContext.getServletContext();
      config = pageContext.getServletConfig();
      session = pageContext.getSession();
      out = pageContext.getOut();
      _jspx_out = out;
      _jspx_resourceInjector = (org.glassfish.jsp.api.ResourceInjector) application.getAttribute("com.sun.appserv.jsp.resource.injector");

      out.write(" \n");
      out.write("\n");
      out.write("\n");
      out.write(" \n");
      out.write(" \n");
      out.write("\n");
      out.write("\n");


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

    } catch (Throwable t) {
      if (!(t instanceof SkipPageException)){
        out = _jspx_out;
        if (out != null && out.getBufferSize() != 0)
          out.clearBuffer();
        if (_jspx_page_context != null) _jspx_page_context.handlePageException(t);
        else throw new ServletException(t);
      }
    } finally {
      _jspxFactory.releasePageContext(_jspx_page_context);
    }
  }
}
