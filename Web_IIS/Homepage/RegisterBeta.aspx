<%@ Page Title="" Language="C#" MasterPageFile="Site.Master" AutoEventWireup="true" CodeBehind="RegisterBeta.aspx.cs" Inherits="Homepage.RegisterBeta" %>

<asp:Content ID="Content3" ContentPlaceHolderID="MainContent" runat="server">

    <div class="container-fluid">
        <div class="row">
            <div class="col-xs-3 col-md-2"></div>
            <div class="col-xs-6 col-md-8">
                <div class="well">
                    <p class="lead" style="margin-top: 2%;">
                        VideoDashboard is currently in public beta. You can download and test the application and its features for free during this period.
                        <br />
                        You can register as beta tester by filling in the form below. Registered beta testers will get a <50%> discount once the beta phase is over.
                        <br />
                    </p>
                    <p class="small">
                        Note that not all features may be available during the beta test. The registration is not required to download and use the software during beta.<br />
                        <br />
                    </p>

                    <div class="visible-md visible-lg hidden-xs hidden-sm">
                        <a href="#" class="btn btn-success btn-lg">Download Now</a>
                    </div>
                    <div class="visible-sm visible-xs hidden-md hidden-lg">
                        <a href="#" class="btn btn-success btn-sm">Download Now</a>
                    </div>
                </div>
            </div>
            <div class="col-xs-3 col-md-2"></div>
        </div>
    </div>
    <div class="container-fluid" style="margin-top: 6%; margin-bottom: 4%;">
        <div class="row">
            <div class="col-xs-3 col-md-4"></div>
            <div class="col-xs-6 col-md-4">
                <form role="form" method="post" autocomplete="on" enctype="application/x-www-form-urlencoded">
                    <asp:Panel runat="server" CssClass="form-group" ID="panelRegFormName">
                        <label for="fieldName">Name</label>
                        <input type="text" class="form-control" id="fieldName" name="UserName" placeholder="Your Name" />
                    </asp:Panel>
                    <asp:Panel runat="server" CssClass="form-group" ID="panelRegFormEmail">
                        <label for="fieldMail">E-mail</label>
                        <input type="email" class="form-control" id="fieldMail" name="Email" placeholder="Your mail" />
                    </asp:Panel>
                    <%--                    <p style="margin-top: 5%; margin-bottom: 4%;" class="small">By clicking the "Register Now button you apply to the <a href="BetaTest.aspx" class="text-info">Terms and Conditions</a> for the beta tester program.</p>--%>
                    <div style="margin-top: 6%;">
                        <button type="submit" class="btn btn-success">Register</button>
                    </div>
                </form>
            </div>
            <div class="col-xs-3 col-md-4 "></div>
        </div>
    </div>
    <asp:Panel runat="server" CssClass="container-fluid">
        <div class="row">
            <div class="col-xs-4 col-md-4"></div>
            <div class="col-xs-4 col-md-4">
                <asp:Label runat="server" CssClass="h4" ID="RegisterStatus">
                    
                </asp:Label>
            </div>
            <div class="col-xs-4 col-md-4"></div>
        </div>
    </asp:Panel>
</asp:Content>
