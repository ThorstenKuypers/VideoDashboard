<%@ Page Title="VideoDashboard" Language="C#" MasterPageFile="Site.Master" AutoEventWireup="true" CodeBehind="Home.aspx.cs" Inherits="Homepage.Home" %>

<asp:Content ContentPlaceHolderID="Head" runat="server">
</asp:Content>
<asp:Content ContentPlaceHolderID="MainContent" runat="server">

    <div class="container-fluid">
        <div class="row">
            <div class="col-xs-2 col-md-2"></div>
            <div class="col-xs-8 col-md-8">
                <div class="pane panel-default">
                    <div class="panel-body">
                        <p class="h4">
                            So, you have recorded telemetry data in your favorite sim and now you want to show those in your
                        latest video containing your racing action?<br />
                            Or do you have an awesome hotlap and want to
                         show how you achieved it?
                        </p>
                        <p class="h4">Now, that is what VideoDashboard is made for.</p>

                        <p class="h4">
                            Take your data and recorded video, throw it into VideoDashboard and this could be the result...

                        </p>
                    </div>
                </div>
            </div>
            <div class="col-xs-2 col-md-2"></div>
        </div>
    </div>

    <div class="container-fluid">
        <div class="row">
            <div class="col-xs-2 col-md-2"></div>
            <div class="col-xs-8 col-md-8">
                <div class="panel-body">
                    <div class="embed-responsive embed-responsive-16by9">
                        <iframe src="//www.youtube.com/embed/vN8KvBgH0JU" allowfullscreen></iframe>
                    </div>
                </div>
            </div>
            <div class="col-md-2"></div>
        </div>
    </div>

    <div class="container-fluid" style="margin-top: 2%;">
        <div class="row">
            <div class="col-xs-3 col-md-3"></div>
            <div class="col-xs-8 col-md-8">
                <p class="h4">Features:</p>

                <div class="col-xs-6 col-md-6">
                    <ul>
                        <li>supports iRacing, rFactor and rFactor2 *</li>
                        <li>broad range of data channels available</li>
                        <li>customizable overlays with simple definition language (similar to CSS)</li>
                        <li>video output in raw or wmv encoded format</li>
                        <li>pre-configured video profiles for YouTube upload</li>
                    </ul>

                    <p class="small">
                        <br />
                        * for rFactor and rFactor2 an additional data logging plugin is needed<br />
                        (available in downloads section)<br />
                        support for other simulations might become available in the future
                    </p>
                </div>
            </div>
            <div class="col-xs-8 col-md-8"></div>

        </div>
    </div>

    <div class="container-fluid">
        <div class="row">
            <div class="col-xs-2 col-md-2"></div>
            <div class="col-xs-8 col-md-8">
                <div class="well well-sm" style="margin-top: 5%; width: 80%; margin-left: auto; margin-right: auto;">
                    <p class="">
                        VideoDashboard is currently in public beta.<br />
                        You can download and test the software for free during this period. After beta period has expired you can buy the software
                        for a small donation. However a free test version will be available.
                    </p>

<%--                    <a href="RegisterBeta.aspx" class="btn btn-success btn-xs visible-xs" style="width: 30%;">Register</a>
                    <a href="RegisterBeta.aspx" class="btn btn-success btn-sm hidden-xs visible-sm" style="width: 20%;">Register</a>
                    <a href="RegisterBeta.aspx" class="btn btn-success btn-lg hidden-xs hidden-sm">Register Now</a>--%>
                </div>
            </div>
            <div class="col-xs-2 col-md-2"></div>
        </div>
    </div>
</asp:Content>
