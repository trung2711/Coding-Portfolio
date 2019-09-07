package model.tournament;

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

    public String tournamentId = "";
    public String tournamentName = "";
    public String tournamentCapacity = "";
    public String tournamentPrize = "";
    public String tournamentDescription = "";
    public String tournamentDate = "";
    public String tournamentTime = "";
    public String tournamentRegistrationDeadline = "";
    public String imageUrl = "";
    public String discordUrl = "";

    public String errorMsg = "";

    // default constructor leaves all data members with empty string (Nothing null).
    public StringData() {
    }

    // overloaded constructor sets all data members by extracting from resultSet.
    public StringData(ResultSet results) {
        try {
            this.tournamentId = FormatUtils.formatInteger(results.getObject("tournament_id"));
            this.tournamentName = FormatUtils.formatString(results.getObject("tournament_name"));
            this.tournamentCapacity = FormatUtils.formatInteger(results.getObject("tournament_capacity"));
            this.tournamentPrize = FormatUtils.formatDollar(results.getObject("tournament_prize"));
            this.tournamentDescription = FormatUtils.formatString(results.getObject("tournament_description"));
            this.tournamentDate = FormatUtils.formatDate(results.getObject("tournament_date"));
            this.tournamentTime = FormatUtils.formatString(results.getObject("tournament_time"));
            this.tournamentRegistrationDeadline = FormatUtils.formatDate(results.getObject("tournament_registration_deadline"));
            this.imageUrl = FormatUtils.formatString(results.getObject("image_url"));
            this.discordUrl = FormatUtils.formatString(results.getObject("discord_url"));
            
        } catch (Exception e) {
            this.errorMsg = "Exception thrown in model.webUser.StringData (the constructor that takes a ResultSet): " + e.getMessage();
        }
    }
    
    public void setNull() {
        this.tournamentId = null;
        this.tournamentName = null;
        this.tournamentCapacity = null;
        this.tournamentPrize = null;
        this.tournamentDescription = null;
        this.tournamentDate = null;
        this.tournamentTime = null;
        this.tournamentRegistrationDeadline = null;
        this.imageUrl = null;
        this.discordUrl = null;
    }

    public int getCharacterCount() {
        String s = this.tournamentId + this.tournamentName + this.tournamentCapacity + this.tournamentPrize
                + this.tournamentDescription + this.tournamentDate + this.tournamentTime + this.tournamentRegistrationDeadline + this.imageUrl + this.discordUrl;
        return s.length();
    }

    public String toString() {
        return "Tournament ID: " + this.tournamentId
                + ", Tournament name: " + this.tournamentName
                + ", Tournament capacity: " + this.tournamentCapacity
                + ", Tournament prize: " + this.tournamentPrize
                + ", Tournament description: " + this.tournamentDescription
                + ", Tournament date: " + this.tournamentDate
                + ", Tournament time: " + this.tournamentTime
                + ", Tournament deadline: " + this.tournamentRegistrationDeadline
                + ", Image URL: " + this.imageUrl
                + ", Discord URL: " + this.discordUrl
                ;
    }
}
