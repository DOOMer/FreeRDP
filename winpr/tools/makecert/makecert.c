/**
 * WinPR: Windows Portable Runtime
 * makecert replacement
 *
 * Copyright 2012 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <winpr/crt.h>
#include <winpr/cmdline.h>

COMMAND_LINE_ARGUMENT_A args[] =
{
	/* Basic Options */

	{ "n", COMMAND_LINE_VALUE_REQUIRED, "<name>", NULL, NULL, -1, NULL,
			"Specifies the subject's certificate name. This name must conform to the X.500 standard."
			"The simplest method is to specify the name in double quotes, preceded by CN=; for example, -n \"CN=myName\"."
	},
	{ "pe", COMMAND_LINE_VALUE_FLAG, NULL, NULL, NULL, -1, NULL,
			"Marks the generated private key as exportable. This allows the private key to be included in the certificate."
	},
	{ "sk", COMMAND_LINE_VALUE_REQUIRED, "<keyname>", NULL, NULL, -1, NULL,
			"Specifies the subject's key container location, which contains the private key."
			"If a key container does not exist, it will be created."
	},
	{ "sr", COMMAND_LINE_VALUE_REQUIRED, "<location>", NULL, NULL, -1, NULL,
			"Specifies the subject's certificate store location. location can be either currentuser (the default) or localmachine."
	},
	{ "ss", COMMAND_LINE_VALUE_REQUIRED, "<store>", NULL, NULL, -1, NULL,
			"Specifies the subject's certificate store name that stores the output certificate."
	},
	{ "#", COMMAND_LINE_VALUE_REQUIRED, "<number>", NULL, NULL, -1, NULL,
			"Specifies a serial number from 1 to 2,147,483,647. The default is a unique value generated by Makecert.exe."
	},
	{ "$", COMMAND_LINE_VALUE_REQUIRED, "<authority>", NULL, NULL, -1, NULL,
			"Specifies the signing authority of the certificate, which must be set to either commercial "
			"(for certificates used by commercial software publishers) or individual (for certificates used by individual software publishers)."
	},

	/* Extended Options */

	{ "a", COMMAND_LINE_VALUE_REQUIRED, "<algorithm>", NULL, NULL, -1, NULL,
			"Specifies the signature algorithm. algorithm must be md5, sha1 (the default), sha256, sha384, or sha512."
	},
	{ "b", COMMAND_LINE_VALUE_REQUIRED, "<mm/dd/yyyy>", NULL, NULL, -1, NULL,
			"Specifies the start of the validity period. Defaults to the current date."
	},
	{ "crl", COMMAND_LINE_VALUE_FLAG, NULL, NULL, NULL, -1, NULL,
			"Generates a certificate relocation list (CRL) instead of a certificate."
	},
	{ "cy", COMMAND_LINE_VALUE_REQUIRED, "<certType>", NULL, NULL, -1, NULL,
			"Specifies the certificate type. Valid values are end for end-entity and authority for certification authority."
	},
	{ "e", COMMAND_LINE_VALUE_REQUIRED, "<mm/dd/yyyy>", NULL, NULL, -1, NULL,
			"Specifies the end of the validity period. Defaults to 12/31/2039 11:59:59 GMT."
	},
	{ "eku", COMMAND_LINE_VALUE_REQUIRED, "<oid[,oid…]>", NULL, NULL, -1, NULL,
			"Inserts a list of comma-separated, enhanced key usage object identifiers (OIDs) into the certificate."
	},
	{ "h", COMMAND_LINE_VALUE_REQUIRED, "<number>", NULL, NULL, -1, NULL,
			"Specifies the maximum height of the tree below this certificate."
	},
	{ "ic", COMMAND_LINE_VALUE_REQUIRED, "<file>", NULL, NULL, -1, NULL,
			"Specifies the issuer's certificate file."
	},
	{ "ik", COMMAND_LINE_VALUE_REQUIRED, "<keyName>", NULL, NULL, -1, NULL,
			"Specifies the issuer's key container name."
	},
	{ "iky", COMMAND_LINE_VALUE_REQUIRED, "<keyType>", NULL, NULL, -1, NULL,
			"Specifies the issuer's key type, which must be one of the following: "
			"signature (which indicates that the key is used for a digital signature), "
			"exchange (which indicates that the key is used for key encryption and key exchange), "
			"or an integer that represents a provider type."
			"By default, you can pass 1 for an exchange key or 2 for a signature key."
	},
	{ "in", COMMAND_LINE_VALUE_REQUIRED, "<name>", NULL, NULL, -1, NULL,
			"Specifies the issuer's certificate common name."
	},
	{ "ip", COMMAND_LINE_VALUE_REQUIRED, "<provider>", NULL, NULL, -1, NULL,
			"Specifies the issuer's CryptoAPI provider name. For information about the CryptoAPI provider name, see the –sp option."
	},
	{ "ir", COMMAND_LINE_VALUE_REQUIRED, "<location>", NULL, NULL, -1, NULL,
			"Specifies the location of the issuer's certificate store. location can be either currentuser (the default) or localmachine."
	},
	{ "is", COMMAND_LINE_VALUE_REQUIRED, "<store>", NULL, NULL, -1, NULL,
			"Specifies the issuer's certificate store name."
	},
	{ "iv", COMMAND_LINE_VALUE_REQUIRED, "<pvkFile>", NULL, NULL, -1, NULL,
			"Specifies the issuer's .pvk private key file."
	},
	{ "iy", COMMAND_LINE_VALUE_REQUIRED, "<type>", NULL, NULL, -1, NULL,
			"Specifies the issuer's CryptoAPI provider type. For information about the CryptoAPI provider type, see the –sy option."
	},
	{ "l", COMMAND_LINE_VALUE_REQUIRED, "<link>", NULL, NULL, -1, NULL,
			"Links to policy information (for example, to a URL)."
	},
	{ "len", COMMAND_LINE_VALUE_REQUIRED, "<number>", NULL, NULL, -1, NULL,
			"Specifies the generated key length, in bits."
	},
	{ "m", COMMAND_LINE_VALUE_REQUIRED, "<number>", NULL, NULL, -1, NULL,
			"Specifies the duration, in months, of the certificate validity period."
	},
	{ "nscp", COMMAND_LINE_VALUE_FLAG, NULL, NULL, NULL, -1, NULL,
			"Includes the Netscape client-authorization extension."
	},
	{ "r", COMMAND_LINE_VALUE_FLAG, NULL, NULL, NULL, -1, NULL,
			"Creates a self-signed certificate."
	},
	{ "sc", COMMAND_LINE_VALUE_REQUIRED, "<file>", NULL, NULL, -1, NULL,
			"Specifies the subject's certificate file."
	},
	{ "sky", COMMAND_LINE_VALUE_REQUIRED, "<keyType>", NULL, NULL, -1, NULL,
			"Specifies the subject's key type, which must be one of the following: "
			"signature (which indicates that the key is used for a digital signature), "
			"exchange (which indicates that the key is used for key encryption and key exchange), "
			"or an integer that represents a provider type. "
			"By default, you can pass 1 for an exchange key or 2 for a signature key."
	},
	{ "sp", COMMAND_LINE_VALUE_REQUIRED, "<provider>", NULL, NULL, -1, NULL,
			"Specifies the subject's CryptoAPI provider name, which must be defined in the registry subkeys of "
			"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography\\Defaults\\Provider. If both –sp and –sy are present, "
			"the type of the CryptoAPI provider must correspond to the Type value of the provider's subkey."
	},
	{ "sv", COMMAND_LINE_VALUE_REQUIRED, "<pvkFile>", NULL, NULL, -1, NULL,
			"Specifies the subject's .pvk private key file. The file is created if none exists."
	},
	{ "sy", COMMAND_LINE_VALUE_REQUIRED, "<type>", NULL, NULL, -1, NULL,
			"Specifies the subject's CryptoAPI provider type, which must be defined in the registry subkeys of "
			"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography\\Defaults\\Provider Types. If both –sy and –sp are present, "
			"the name of the CryptoAPI provider must correspond to the Name value of the provider type subkey."
	},
	{ "tbs", COMMAND_LINE_VALUE_REQUIRED, "<file>", NULL, NULL, -1, NULL,
			"Specifies the certificate or CRL file to be signed."
	},

	/* Help */

	{ "?", COMMAND_LINE_VALUE_FLAG | COMMAND_LINE_PRINT_HELP, NULL, NULL, NULL, -1, "help", "print help" },
	{ "!", COMMAND_LINE_VALUE_FLAG | COMMAND_LINE_PRINT_HELP, NULL, NULL, NULL, -1, "help-ext", "print extended help" },
	{ NULL, 0, NULL, NULL, NULL, -1, NULL, NULL }
};

int makecert_print_command_line_help(int argc, char** argv)
{
	char* str;
	int length;
	COMMAND_LINE_ARGUMENT_A* arg;

	printf("Usage: %s [options] [output file]\n", argv[0]);
	printf("\n");

	arg = args;

	do
	{
		if (arg->Flags & COMMAND_LINE_VALUE_FLAG)
		{
			printf("    %s", "-");
			printf("%-20s", arg->Name);
			printf("\t%s\n", arg->Text);
		}
		else if ((arg->Flags & COMMAND_LINE_VALUE_REQUIRED) || (arg->Flags & COMMAND_LINE_VALUE_OPTIONAL))
		{
			printf("    %s", "-");

			if (arg->Format)
			{
				length = strlen(arg->Name) + strlen(arg->Format) + 2;
				str = malloc(length + 1);
				sprintf_s(str, length + 1, "%s %s", arg->Name, arg->Format);
				printf("%-20s", str);
				free(str);
			}
			else
			{
				printf("%-20s", arg->Name);
			}

			printf("\t%s\n", arg->Text);
		}
	}
	while ((arg = CommandLineFindNextArgumentA(arg)) != NULL);

	return 1;
}

int main(int argc, char* argv[])
{
	int status;
	DWORD flags;
	COMMAND_LINE_ARGUMENT_A* arg;

	flags = COMMAND_LINE_SEPARATOR_SPACE | COMMAND_LINE_SIGIL_DASH;
	status = CommandLineParseArgumentsA(argc, (const char**) argv, args, flags, NULL, NULL, NULL);

	if (status == COMMAND_LINE_STATUS_PRINT_HELP)
	{
		makecert_print_command_line_help(argc, argv);
		return 0;
	}

	arg = args;

	do
	{
		if (!(arg->Flags & COMMAND_LINE_VALUE_PRESENT))
			continue;

		if (arg->Flags & COMMAND_LINE_VALUE_REQUIRED)
			printf("Argument: %s Value: %s\n", arg->Name, arg->Value);
		else
			printf("Argument: %s\n", arg->Name);

		CommandLineSwitchStart(arg)

		/* Basic Options */

		CommandLineSwitchCase(arg, "n")
		{

		}
		CommandLineSwitchCase(arg, "pe")
		{

		}
		CommandLineSwitchCase(arg, "sk")
		{

		}
		CommandLineSwitchCase(arg, "sr")
		{

		}
		CommandLineSwitchCase(arg, "ss")
		{

		}
		CommandLineSwitchCase(arg, "#")
		{

		}
		CommandLineSwitchCase(arg, "$")
		{

		}

		/* Extended Options */

		CommandLineSwitchCase(arg, "a")
		{

		}
		CommandLineSwitchCase(arg, "b")
		{

		}
		CommandLineSwitchCase(arg, "crl")
		{

		}
		CommandLineSwitchCase(arg, "cy")
		{

		}
		CommandLineSwitchCase(arg, "e")
		{

		}
		CommandLineSwitchCase(arg, "eku")
		{

		}
		CommandLineSwitchCase(arg, "h")
		{

		}
		CommandLineSwitchCase(arg, "ic")
		{

		}
		CommandLineSwitchCase(arg, "ik")
		{

		}
		CommandLineSwitchCase(arg, "iky")
		{

		}
		CommandLineSwitchCase(arg, "in")
		{

		}
		CommandLineSwitchCase(arg, "ip")
		{

		}
		CommandLineSwitchCase(arg, "ir")
		{

		}
		CommandLineSwitchCase(arg, "is")
		{

		}
		CommandLineSwitchCase(arg, "iv")
		{

		}
		CommandLineSwitchCase(arg, "iy")
		{

		}
		CommandLineSwitchCase(arg, "l")
		{

		}
		CommandLineSwitchCase(arg, "l")
		{

		}
		CommandLineSwitchCase(arg, "m")
		{

		}
		CommandLineSwitchCase(arg, "nscp")
		{

		}
		CommandLineSwitchCase(arg, "r")
		{

		}
		CommandLineSwitchCase(arg, "sc")
		{

		}
		CommandLineSwitchCase(arg, "sky")
		{

		}
		CommandLineSwitchCase(arg, "sp")
		{

		}
		CommandLineSwitchCase(arg, "sv")
		{

		}
		CommandLineSwitchCase(arg, "sy")
		{

		}
		CommandLineSwitchCase(arg, "tbs")
		{

		}

		CommandLineSwitchDefault(arg)
		{

		}

		CommandLineSwitchEnd(arg)
	}
	while ((arg = CommandLineFindNextArgumentA(arg)) != NULL);

	return 0;
}
