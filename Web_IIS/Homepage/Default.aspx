<%@ Page Title="VideoDashboard" Language="C#" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="Homepage.Default" %>

<!DOCTYPE html>

<html>
<head>
    <meta charset="utf-8" />
    <meta lang="en" />
    <meta name="viewport" content="width-device-width, initial-scale=1.0, maximum-scale=1.0, user-scaleable=no" />

    <title>VideoDashboard Home</title>
    <link href="Content/bootstrap.css" rel="stylesheet" />
    <link href="Content/bs-theme-metal-gray.css" rel="stylesheet" />
    <link href="Content/main.css" rel="stylesheet" />
</head>

<body>

    <div class="container-fluid">
        <div class="row">
            <div class="col-xs-1"></div>
            <div class="col-xs-10">
                <div class="carousel-container">
                    <div id="myCarousel" class="carousel slide" data-ride="carousel" data-interval="3000">
                        <!-- Indicators -->
                        <ol class="carousel-indicators">
                            <li data-target="#myCarousel" data-slide-to="0" class="active"></li>
                            <li data-target="#myCarousel" data-slide-to="1"></li>
                            <li data-target="#myCarousel" data-slide-to="2"></li>
                        </ol>

                        <!-- Wrapper for slides -->
                        <div class="carousel-inner">
                            <div class="item active">
                                <img src="images/ir.gif" alt="...">
                            </div>
                            <div class="item">
                                <img src="images/rf2.gif" alt="...">
                            </div>
                            <div class="item">
                                <img src="images/rf.gif" alt="..." />

                            </div>
                        </div>

                        <!-- Controls -->
                        <a class="left carousel-control" href="#myCarousel" role="button" data-slide="prev">
                            <span class="glyphicon glyphicon-chevron-left"></span>
                        </a>
                        <a class="right carousel-control" href="#myCarousel" role="button" data-slide="next">
                            <span class="glyphicon glyphicon-chevron-right"></span>
                        </a>
                    </div>

                    <div class="container">
                        <div class="title-caption">
                            <p class="h1">Welcome to VideoDashboard</p>
                        </div>
                        <div class="title-sub">
                            <p class="h3">Add a visual representation of telemetry data to your sim racing videos in form of customizable overlays.</p>
                        </div>
                        <div class="visible-md visible-lg">
                            <div class=" enter-btn">
                                <div><a href="Home.aspx" class="btn btn-success btn-lg">Learn More</a></div>
                            </div>
                        </div>
                        <div class="visible-sm visible-xs">
                            <div class="enter-btn">
                                <div><a href="Home.aspx" class="btn btn-success btn-sm">Learn More</a></div>
                            </div>
                        </div>
<%--                        <div class="intro-textbox">
                        </div>--%>
                    </div>
                </div>
            </div>

        </div>
        <div class="col-xs-1"></div>
    </div>

    <script src="Scripts/jquery-1.9.0.min.js"></script>
    <script src="Scripts/bootstrap.min.js"></script>
</body>
</html>
