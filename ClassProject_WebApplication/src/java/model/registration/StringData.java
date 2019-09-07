package model.registration;

import dbUtils.FormatUtils;
import java.sql.ResultSet;


/* The purpose of this class is just to "bundle together" all the 
 * character data that the user might type in when they want to 
 * add a new Customer or edit an existing customer.  This String
 * data is "pre-validated" data, meaning they might have typed 
 * in a character string where a number was expected.
 * 
 * There are no getter or setter methods since we are not trying to
 * protect this data in any way.  We want to let the JSP page have
 * free access to put data in or take it out. */
public class StringData {
    public String registrationId = "";
    public String registrationDate = "";
    public String webUserId = "";
    public String userEmail = "";
    public String additionalNotes = "";
    public String percentAsDonation = "";
    public String tournamentId = "";
    public String tournamentName = "";
    public String tournamentCapacity = "";
    public String tournamentPrize = "";
    public String tournamentDate = "";
    public String tournamentTime = "";
    public String tournamentRegistrationDeadline = "";
    
    public String errorMsg = "";

    // default constructor leaves all data members with empty string (Nothing null).
    public StringData() {
    }

    // overloaded constructor sets all data members by extracting from resultSet.
    public StringData(ResultSet results) {
        try {
            this.registrationId = FormatUtils.formatInteger(results.getObject("registration_id"));
            this.registrationDate = FormatUtils.formatDate(results.getObject("registration_date"));
            this.userEmail = FormatUtils.formatString(results.getObject("user_email"));
            this.percentAsDonation = FormatUtils.formatInteger(results.getObject("percent_as_donation"));
            this.additionalNotes = FormatUtils.formatString(results.getObject("additional_notes"));
            this.tournamentName = FormatUtils.formatString(results.getObject("tournament_name"));
            this.tournamentCapacity = FormatUtils.formatInteger(results.getObject("tournament_capacity"));
            this.tournamentPrize = FormatUtils.formatDollar(results.getObject("tournament_prize"));
            this.tournamentDate = FormatUtils.formatDate(results.getObject("tournament_date"));
            this.tournamentTime = FormatUtils.formatString(results.getObject("tournament_time"));
            this.tournamentRegistrationDeadline = FormatUtils.formatDate(results.getObject("tournament_registration_deadline"));
            
        } catch (Exception e) {
            this.errorMsg = "Exception thrown in model.registration.StringData (the constructor that takes a ResultSet): " + e.getMessage();
        }
    }

    public int getCharacterCount() {
        String s = this.registrationId + this.registrationDate + this.userEmail + this.percentAsDonation + this.additionalNotes + this.tournamentName + this.tournamentCapacity + this.tournamentPrize
                 + this.tournamentDate + this.tournamentTime + this.tournamentRegistrationDeadline;
        return s.length();
    }

    public String toString() {
        return  "Registration ID: "+this.registrationId
                + ", Registration date: " + this.registrationDate
                + ", User email: " + this.userEmail
                + ", Percent as donation: " + this.percentAsDonation
                + ", Additional notes: " + this.additionalNotes
                + ", Tournament name: " + this.tournamentName
                + ", Tournament capacity: " + this.tournamentCapacity
                + ", Tournament prize: " + this.tournamentPrize
                + ", Tournament date: " + this.tournamentDate
                + ", Tournament time: " + this.tournamentTime
                + ", Tournament deadline: " + this.tournamentRegistrationDeadline
                ;
    }
}
