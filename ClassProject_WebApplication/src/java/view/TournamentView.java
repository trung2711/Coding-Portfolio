/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package view;

import dbUtils.DbConn;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import model.tournament.StringData;
import model.tournament.StringDataList;

/**
 *
 * @author Trung Nguyen
 */
public class TournamentView {
    public static StringDataList allUsersAPI(DbConn dbc) {

        //PreparedStatement stmt = null;
        //ResultSet results = null;
        StringDataList sdl = new StringDataList();
        try {
            String sql = "SELECT tournament_id, tournament_name, image_url, tournament_capacity, tournament_prize, tournament_description, "+
                    "tournament_date, tournament_time, tournament_registration_deadline, discord_url "+
                    "FROM tournament " + 
                    "ORDER BY tournament_id ";  // you always want to order by something, not just random order.
            PreparedStatement stmt = dbc.getConn().prepareStatement(sql);
            ResultSet results = stmt.executeQuery();
            while (results.next()) {
                sdl.add(results);
            }
            results.close();
            stmt.close();
        } catch (Exception e) {
            StringData sd = new StringData();
            sd.errorMsg = "Exception thrown in WebUserView.allUsersAPI(): " + e.getMessage();
            sdl.add(sd);
        }
        return sdl;
    }
}
