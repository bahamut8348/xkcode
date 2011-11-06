void Base64_Code( unsigned char *chsrc, unsigned int srclen ,unsigned char *chdes )
{
	char chadd[3] = { 0 };
	unsigned char temp[4] = { 0 };
	unsigned char t = 0;
	int len = 0;
	int i = 0;
	int n = 0;
	len= srclen;
	while (len > 0)
	{
		// temp[0]	| temp[2]	|  temp[3]	| temp[4]
		// xxxxxx	| xx xxxx	|  xxxx xx	| xxxxxx
		
		temp[0] = (*chsrc) >> 2;
		
		if (len >= 3)
		{
			t = (*chsrc & 0x03) << 4;
			temp[1] = t | (*(chsrc+1) >> 4);

			t = ((*(chsrc+1) & 0x0f) << 2);
			temp[2] = t | (*(chsrc+2) >> 6);

			temp[3] = (*(chsrc+2) & 0x3f );
			n = 3;
		}
		else if (len >= 2)
		{
			t = (*chsrc & 0x03) << 4;
			temp[1] = t | (*(chsrc+1) >> 4);

			t = ((*(chsrc+1) & 0x0f) << 2);
			temp[2] = t ;
			n = 2;
		}
		else
		{
			t = (*chsrc & 0x03) << 4;
			temp[1] = t ;
			n = 1;
		}
		
		
		for (i = 0;i < 4;i++)
		{
			if (i <= n)
			{
				if(temp[i]>=0 && temp[i]<=25)
					*(chdes+i)=temp[i]+65;
				if(temp[i]>=26 && temp[i]<=51)
					*(chdes+i)=temp[i]+71;
				if(temp[i]>=52 && temp[i]<=61)
					*(chdes+i)=temp[i]-4;
				if(temp[i]==62)
					*(chdes+i)=43;
				if(temp[i]==63)
					*(chdes+i)=47;
			}
			else
			{
				*(chdes+i) = '=';
			}

		}

		len-=3;
		chsrc+=3;
		chdes+=4;
	}

	*chdes='\0';
}

int main()
{


	return 1;

}