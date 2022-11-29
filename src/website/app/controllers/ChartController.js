const fb = require("firebase");
const url = require('url');
class ChartController 
{
    // [GET] /
    index(req, res, next) 
    {
        if (req.session.loggedin) {
           res.render("chart");
        }
        else {
            res.redirect(url.format({
                pathname: "/",
                query: {
                    "notYetLogin": true,
                }
            }));
        }
    }
}

module.exports = new ChartController;