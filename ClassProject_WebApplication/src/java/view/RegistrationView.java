/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package view;

import dbUtils.DbConn;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import model.registration.StringData;
import model.registration.StringDataList;

/**
 *
 * @author Trung Nguyen
 */
public class RegistrationView {
    public static StringDataList allUsersAPI(DbConn dbc) {

        //PreparedStatement stmt = null;
        //ResultSet results = null;
        StringDataList sdl = new StringDataList();
        try {
            String sql = "SELECT registration_id, registration_date, user_email, percent_as_donation, additional_notes, tournament_name, tournament_capacity, tournament_prize, tournament_date,\n" +
                          "tournament_time, tournament_registration_deadline \n" +
                          "FROM registration, tournament, web_user\n" +
                          "WHERE registration.tournament_id = tournament.tournament_id AND registration.web_user_id = web_user.web_user_id \n" +
                          "ORDER BY registration_id, user_email;\n" +
"  ";  // you always want to order by something, not just random order.
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
