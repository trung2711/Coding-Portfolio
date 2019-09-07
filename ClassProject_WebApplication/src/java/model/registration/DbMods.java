/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package model.registration;

import dbUtils.*;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class DbMods {
    
    private static StringData validate(StringData inputData) {

        StringData errorMsgs = new StringData();

        /*public String registrationId = "";
        public String registrationDate = "";
        public String userEmail = "";
        public String additionalNotes = "";
        public String percentAsDonation = "";
        public String tournamentName = "";
        public String tournamentCapacity = "";
        public String tournamentPrize = "";
        public String tournamentDate = "";
        public String tournamentTime = "";
        public String tournamentRegistrationDeadline = "";
    
        public String errorMsg = "";*/
        
        errorMsgs.percentAsDonation = ValidationUtils.integerValidationMsg(inputData.percentAsDonation, false);
        errorMsgs.registrationDate = ValidationUtils.dateValidationMsg(inputData.registrationDate, true);
        errorMsgs.additionalNotes = ValidationUtils.stringValidationMsg(inputData.additionalNotes, 300, false);

        return errorMsgs;
    } // validate 
      
    public static StringData insert(StringData inputData, DbConn dbc) {

        StringData errorMsgs = new StringData();
        errorMsgs = validate(inputData);
        if (errorMsgs.getCharacterCount() > 0) {  // at least one field has an error, don't go any further.
            errorMsgs.errorMsg = "Please try again";
            return errorMsgs;

        } else { // all fields passed validation

            /*
                       String sql = "SELECT web_user_id, user_email, user_password, membership_fee, birthday, "+
                    "web_user.user_role_id, user_role_type "+
                    "FROM web_user, user_role where web_user.user_role_id = user_role.user_role_id " + 
                    "ORDER BY web_user_id ";
             */
            // Start preparing SQL statement
            String sql = "INSERT INTO registration (registration_date, percent_as_donation, additional_notes, web_user_id, tournament_id) "
                    + "values (?,?,?,?,?)";

            // PrepStatement is Sally's wrapper class for java.sql.PreparedStatement
            // Only difference is that Sally's class takes care of encoding null 
            // when necessary. And it also System.out.prints exception error messages.
            PrepStatement pStatement = new PrepStatement(dbc, sql);

            // Encode string values into the prepared statement (wrapper class).
            pStatement.setDate(1, ValidationUtils.dateConversion(inputData.registrationDate)); // string type is simple
            pStatement.setInt(2, ValidationUtils.integerConversion(inputData.percentAsDonation));
            pStatement.setString(3, inputData.additionalNotes);
            pStatement.setInt(4, ValidationUtils.integerConversion(inputData.webUserId));
            pStatement.setInt(5, ValidationUtils.integerConversion(inputData.tournamentId));

            // here the SQL statement is actually executed
            int numRows = pStatement.executeUpdate();

            // This will return empty string if all went well, else all error messages.
            errorMsgs.errorMsg = pStatement.getErrorMsg();
            if (errorMsgs.errorMsg.length() == 0) {
                if (numRows == 1) {
                    errorMsgs.errorMsg = ""; // This means SUCCESS. Let the user interface decide how to tell this to the user.
                } else {
                    // probably never get here unless you forgot your WHERE clause and did a bulk sql update.
                    errorMsgs.errorMsg = numRows + " records were inserted when exactly 1 was expected.";
                }
            } else if (errorMsgs.errorMsg.contains("foreign key")) {
                errorMsgs.errorMsg = "Invalid Web User or Tournament Id";
            }
        } // customerId is not null and not empty string.
        return errorMsgs;
    } // insert
    
    public static String delete(String registrationId, DbConn dbc) {

        if (registrationId == null) {
            return "Programmer error: cannot attempt to delete web_user record that matches null user id";
        }
        // This method assumes that the calling Web API (JSP page) has already confirmed 
        // that the database connection is OK. BUT if not, some reasonable exception should 
        // be thrown by the DB and passed back anyway... 
        String result = ""; // empty string result means the delete worked fine.
        try {

            String sql = "DELETE FROM registration WHERE registration_id = ?";

            // This line compiles the SQL statement (checking for syntax errors against your DB).
            PreparedStatement pStatement = dbc.getConn().prepareStatement(sql);

            // Encode user data into the prepared statement.
            pStatement.setString(1, registrationId);

            int numRowsDeleted = pStatement.executeUpdate();

            if (numRowsDeleted == 0) {
                result = "Programmer Error: did not delete the record with tournament_id " + registrationId;
            } else if (numRowsDeleted > 1) {
                result = "Programmer Error: > 1 record deleted. Did you forget the WHERE clause?";
            }

        } catch (Exception e) {
            result = "Exception thrown in model.tournament.DbMods.delete(): " + e.getMessage();
        }

        return result;
    } //delete

} // class