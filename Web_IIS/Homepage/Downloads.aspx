<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Downloads.aspx.cs" Inherits="Homepage.Downloads" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <div class="container-fluid" style="margin-top: 3%;">
        <div class="col-xs-3 col-md-4"></div>
        <div class="col-xs-6 col-md-4 h4">Download VideoDashboard</div>
        <div class="col-xs-3 col-md-4"></div>
    </div>
    <div class="container-fluid">
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

    <div class="container-fluid" style="margin-top: 1%;">
        <div class="row">
            <div class="col-xs-3 col-md-4"></div>
            <div class="col-xs-6 col-md-4">
                <table class="table h4">
                    <tr>
                        <td><a href="Downloads/VideoDashboard_x86.msi">VideoDashboard 32bit</a></td>
                    </tr>
                    <tr>
                        <td><a href="Downloads/VideoDashboard_x64.msi">VideoDashboard 64bit</a></td>
                    </tr>
                </table>
            </div>
        </div>
        <div class="col-xs-3 col-md-4"></div>
    </div>

    <div class="container-fluid">
        <div class="row">
            <div class="col-xs-2 col-md-3"></div>
            <div class="col-xs-8 col-md-6">
                <div class="pane panel-default panel-body" style="margin-top: 8%; margin-bottom: -2%;">
                    <p class="lead">Here are some other useful downloads for VideoDashboard</p>
                </div>
            </div>
            <div class="col-xs-2 col-md-3"></div>

        </div>
        <div class="row">
            <div class="separator"></div>
        </div>

        <div class="row">
            <div class="col-xs-2 col-md-3"></div>
            <div class="col-xs-8 col-md-6">

                <div class="container-fluid" style="margin-bottom: 25%;">
                    <div class="panel-group" id="accordion">
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <div class="dl-btn-container">
                                    <a href="#" class="btn btn-sm btn-primary">Download</a>
                                </div>
                                <h4 class="panel-title">
                                    <a data-toggle="collapse" data-parent="#accordion" href="#collapseOne">OpenDataLogger
                                    </a>

                                </h4>
                            </div>
                            <div id="collapseOne" class="panel-collapse collapse">
                                <div class="panel-body">
                                    OpenDataLogger provides a set of plugins for rFactor and rFactor2, implementing the OpenGameAPI, which create
                                log files in the ibt format. The OpenDataLogger is free and open source.<br />
                                    Source code is available at <a href="http://github.com/ThorstenKuypers/OpenDataLogger" target="_blank">GitHub</a>.
                                </div>
                            </div>
                        </div>
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <div class="dl-btn-container">
                                    <a href="#" class="btn btn-sm btn-primary">Download</a>
                                </div>

                                <h4 class="panel-title">
                                    <a data-toggle="collapse" data-parent="#accordion" href="#collapseTwo">Notepad++ language file
                                    </a>
                                </h4>
                            </div>
                            <div id="collapseTwo" class="panel-collapse collapse">
                                <div class="panel-body">
                                    This file is an addon for the free and open source text editor Notepad++. It helps you in writing layout files for
                                VideoDashboard by using syntax highlighting.
     
                                </div>
                            </div>
                        </div>
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <div class="dl-btn-container">
                                    <a href="http://downloads.ddigest.com/software/getdownload.php?sid=730&did=1&code=SrW0snic&decode=d03c321cf53697d22d1ba004db4951f7" class="btn btn-sm btn-primary">Download</a>
                                </div>

                                <h4 class="panel-title">
                                    <a data-toggle="collapse" data-parent="#accordion" href="#collapseThree">HuffYUV
                                    </a>
                                </h4>
                            </div>
                            <div id="collapseThree" class="panel-collapse collapse">
                                <div class="panel-body">
                                    HuffYUV is a fast lossless compression codec. Use it when you want to create raw videos without loss of quality
                                which are then used in other video projects eg. Vegas.<br />
                                    Source code is available <a href="http://www.divx-digest.com/software/huffyuv.html" target="_blank">here</a>.
     
                                </div>
                            </div>
                        </div>
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <div class="dl-btn-container">
                                    <a href="http://lags.leetcode.net/LagarithSetup_1327.exe" class="btn btn-sm btn-primary">Download</a>
                                </div>

                                <h4 class="panel-title">
                                    <a data-toggle="collapse" data-parent="#accordion" href="#collapseFour">Lagarith
                                    </a>
                                </h4>
                            </div>
                            <div id="collapseFour" class="panel-collapse collapse">
                                <div class="panel-body">
                                    Lagarith is also a lossless compression codec similar to HuffYUV however it has multi-threading support
                                and better compression and performance than HuffYUV.
                                Use it when you want to create raw videos without loss of quality
                                which are then used in other video projects eg. Vegas.<br />
                                    Source code is available <a href="http://lags.leetcode.net/codec.html" target="_blank">here</a>.
     
                                </div>
                            </div>
                        </div>

                    </div>
                </div>
            </div>
            <div class="col-xs-2 col-md-3"></div>

        </div>
    </div>

    <%--    <div class="row">
        <div class="col-md-3"></div>
        <div class="col-md-6">
            <div class="container-fluid>">
                <div class="panel-group" id="accordion">
                    <div class="panel panel-default panel-heading">
                        <h4 class="panle-title">
                            <a data-toggle="collapse" data-parent="#accordion" href="#collapseOne">OpenDataLogger</a>

                        </h4>
                    </div>
                    <div id="collapseOne" class="panel-collapse collapse in">
                        <div class="panel-body">
                            Text
                        </div>
                    </div>

                </div>
            </div>
        </div>
        <div class="col-md-3"></div>

    </div>--%>
</asp:Content>
