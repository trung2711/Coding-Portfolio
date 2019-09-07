package model.tournament;

import dbUtils.*;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class DbMods {
    
    private static StringData validate(StringData inputData) {

        StringData errorMsgs = new StringData();
        
        errorMsgs.tournamentName = ValidationUtils.stringValidationMsg(inputData.tournamentName, 45, true);
        errorMsgs.tournamentCapacity = ValidationUtils.integerValidationMsg(inputData.tournamentCapacity, true);
        errorMsgs.tournamentPrize = ValidationUtils.decimalValidationMsg(inputData.tournamentPrize, true);
        errorMsgs.tournamentDescription = ValidationUtils.stringValidationMsg(inputData.tournamentDescription, 500, true);

        //optional fields
        errorMsgs.tournamentDate = ValidationUtils.dateValidationMsg(inputData.tournamentDate, false);
        errorMsgs.tournamentTime = ValidationUtils.stringValidationMsg(inputData.tournamentTime, 10 , false);
        errorMsgs.tournamentRegistrationDeadline = ValidationUtils.dateValidationMsg(inputData.tournamentRegistrationDeadline, false);
        errorMsgs.imageUrl = ValidationUtils.stringValidationMsg(inputData.imageUrl, 300 , false);
        errorMsgs.discordUrl = ValidationUtils.stringValidationMsg(inputData.discordUrl, 300 , false);

        return errorMsgs;
    } // validate 
      
    public static StringData insert(StringData inputData, DbConn dbc) {

        StringData errorMsgs = new StringData();
        errorMsgs = validate(inputData);
        if (errorMsgs.getCharacterCount() > 0) {  // at least one field has an error, don't go any further.
            errorMsgs.errorMsg = "Please try again";
            return errorMsgs;

        } else { // all fields passed validation
            
            String sql = "INSERT INTO tournament (tournament_name, tournament_capacity, tournament_prize, tournament_description, tournament_date, tournament_time, tournament_registration_deadline, image_url, discord_url) "
                        + "values (?,?,?,?,?,?,?,?,?)";

            // PrepStatement is Sally's wrapper class for java.sql.PreparedStatement
            // Only difference is that Sally's class takes care of encoding null 
            // when necessary. And it also System.out.prints exception error messages.
            PrepStatement pStatement = new PrepStatement(dbc, sql);

            // Encode string values into the prepared statement (wrapper class).
            pStatement.setString(1, inputData.tournamentName); // string type is simple
                pStatement.setInt(2, ValidationUtils.integerConversion(inputData.tournamentCapacity));
                pStatement.setBigDecimal(3, ValidationUtils.decimalConversion(inputData.tournamentPrize));
                pStatement.setString(4, inputData.tournamentDescription);
                pStatement.setDate(5, ValidationUtils.dateConversion(inputData.tournamentDate));
                pStatement.setString(6, inputData.tournamentTime);
                pStatement.setDate(7, ValidationUtils.dateConversion(inputData.tournamentRegistrationDeadline));
                pStatement.setString(8, inputData.imageUrl);
                pStatement.setString(9, inputData.discordUrl);

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
            } else if (errorMsgs.errorMsg.contains("Duplicate entry")) {
                errorMsgs.errorMsg = "That tournament name is already taken";
            }
        } // customerId is not null and not empty string.
        return errorMsgs;
    } // insert
    
    public static String delete(String tournamentId, DbConn dbc) {

        if (tournamentId == null) {
            return "Programmer error: cannot attempt to delete web_user record that matches null user id";
        }
        // This method assumes that the calling Web API (JSP page) has already confirmed 
        // that the database connection is OK. BUT if not, some reasonable exception should 
        // be thrown by the DB and passed back anyway... 
        String result = ""; // empty string result means the delete worked fine.
        try {

            String sql = "DELETE FROM tournament WHERE tournament_id = ?";

            // This line compiles the SQL statement (checking for syntax errors against your DB).
            PreparedStatement pStatement = dbc.getConn().prepareStatement(sql);

            // Encode user data into the prepared statement.
            pStatement.setString(1, tournamentId);

            int numRowsDeleted = pStatement.executeUpdate();

            if (numRowsDeleted == 0) {
                result = "Programmer Error: did not delete the record with tournament_id " + tournamentId;
            } else if (numRowsDeleted > 1) {
                result = "Programmer Error: > 1 record deleted. Did you forget the WHERE clause?";
            }

        } catch (Exception e) {
            result = "Exception thrown in model.tournament.DbMods.delete(): " + e.getMessage();
        }

        return result;
    } //delete
    
    public static StringData update(StringData inputData, DbConn dbc) {

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

            String sql = "UPDATE tournament SET tournament_name = ?, tournament_capacity = ?, tournament_prize = ?, tournament_description = ?, tournament_date = ?, tournament_time = ?, tournament_registration_deadline = ?, image_url = ?, discord_url = ?" +
                    " WHERE tournament_id = ?";

            // PrepStatement is Sally's wrapper class for java.sql.PreparedStatement
            // Only difference is that Sally's class takes care of encoding null 
            // when necessary. And it also System.out.prints exception error messages.
            PrepStatement pStatement = new PrepStatement(dbc, sql);

            // Encode string values into the prepared statement (wrapper class).
            pStatement.setString(1, inputData.tournamentName); // string type is simple
            pStatement.setInt(2, ValidationUtils.integerConversion(inputData.tournamentCapacity));
            pStatement.setBigDecimal(3, ValidationUtils.decimalConversion(inputData.tournamentPrize));
            pStatement.setString(4, inputData.tournamentDescription);
            pStatement.setDate(5, ValidationUtils.dateConversion(inputData.tournamentDate));
            pStatement.setString(6, inputData.tournamentTime);
            pStatement.setDate(7, ValidationUtils.dateConversion(inputData.tournamentRegistrationDeadline));
            pStatement.setString(8, inputData.imageUrl);
            pStatement.setString(9, inputData.discordUrl);
            pStatement.setInt(10, ValidationUtils.integerConversion(inputData.tournamentId));

            // here the SQL statement is actually executed
            int numRows = pStatement.executeUpdate();

            // This will return empty string if all went well, else all error messages.
            errorMsgs.errorMsg = pStatement.getErrorMsg();
            if (errorMsgs.errorMsg.length() == 0) {
                if (numRows == 1) {
                    errorMsgs.errorMsg = ""; // This means SUCCESS. Let the user interface decide how to tell this to the user.
                } else {
                    // probably never get here unless you forgot your WHERE clause and did a bulk sql update.
                    errorMsgs.errorMsg = numRows + " records were updated (expected to update one record).";
                }
            } else if (errorMsgs.errorMsg.contains("Duplicate entry")) {
                errorMsgs.errorMsg = "That tournament name is already taken";
            }

        } // customerId is not null and not empty string.
        return errorMsgs;
    } // update

    
} // class