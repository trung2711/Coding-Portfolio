package model.tournament;

import dbUtils.*;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class Search {

    public static StringData getTournamentById(DbConn dbc, String id) {

        //PreparedStatement stmt = null;
        //ResultSet results = null;
        StringData sd = new StringData();
        try {
            String sql = "SELECT tournament_id, tournament_name, tournament_capacity, tournament_prize, tournament_description, tournament_registration_deadline, "
                    + "tournament_date, tournament_time, image_Url, discord_Url " 
                    + "FROM tournament WHERE tournament_id = ?";

            PreparedStatement stmt = dbc.getConn().prepareStatement(sql);

            // Encode the id (that the user typed in) into the select statement, into the first 
            // (and only) ? 
            stmt.setString(1, id);

            ResultSet results = stmt.executeQuery();
            if (results.next()) { // id is unique, one or zero records expected in result set
                sd = new StringData(results);
            }
            results.close();
            stmt.close();
        } catch (Exception e) {
            sd.errorMsg = "Exception thrown in getTournamentById(): " + e.getMessage();
        }
        return sd;
    }
} // class
