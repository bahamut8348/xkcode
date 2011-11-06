USE [PSWEB]
GO
/****** 对象:  Table [dbo].[locations]    脚本日期: 06/08/2007 16:18:50 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[locations](
	[id] [int] NOT NULL,
	[name] [nvarchar](max) COLLATE Chinese_PRC_CI_AS NOT NULL,
	[short] [nvarchar](50) COLLATE Chinese_PRC_CI_AS NOT NULL
) ON [PRIMARY]
